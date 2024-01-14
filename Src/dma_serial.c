#include "dma_serial.h"
#include <string.h>

// USB Serial Functions using DMA for transmit
//
// Based on 30010_io.c

volatile uint8_t UART_BUFFER[UART_BUFFER_LENGTH] = {0};
volatile uint8_t UART_END_IDX = 0;
volatile uint8_t UART_START_IDX = 0;
volatile uint8_t UART_COUNT = 0;

// Set to 1 to use the DMA transmit functions or 0 to use the PIO transmit functions
#define USE_DMA 1

// Size of transmit buffer, 4kb should be sufficient for a complete screen update.
// If the buffer is full, the transmit function will busy-wait for a DMA to finish.
#define DMA_TX_BUFSIZE 4096

#if USE_DMA
// Ring buffer used for DMA transmission
typedef struct {
    uint8_t buffer[DMA_TX_BUFSIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t dma_len;
} ringbuffer_t;

ringbuffer_t dma_tx_buf;

// DMA channel
DMA_Channel_TypeDef* uart_dma_channel;


// Start the DMA transfer if there's data to transmit and
// it's not already running.
// Call from ISR or with DMA interrupt disabled.
static void start_dma()
{
	// Return if DMA already running
	if (uart_dma_channel->CCR & DMA_CCR_EN)
		return;
	// Return if there's nothing to send
	if (dma_tx_buf.head == dma_tx_buf.tail)
		return;
	// Calculate length of DMA transfer
	if (dma_tx_buf.head > dma_tx_buf.tail) {
		dma_tx_buf.dma_len = dma_tx_buf.head-dma_tx_buf.tail;
	} else {
		dma_tx_buf.dma_len = DMA_TX_BUFSIZE-dma_tx_buf.tail;
	}
	// Write start address
	uart_dma_channel->CMAR = (uint32_t)(&(dma_tx_buf.buffer[dma_tx_buf.tail]));
	// Write length
	uart_dma_channel->CNDTR = dma_tx_buf.dma_len;
	// Kick off DMA
	uart_dma_channel->CCR |= DMA_CCR_EN;
}

void DMA1_CH7_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF7) // Check if transfer complete flag is set
    {
        DMA1->IFCR |= DMA_IFCR_CTCIF7; // Clear the interrupt flag
        uart_dma_channel->CCR &= ~DMA_CCR_EN; // Disable DMA
        // Update ring buffer tail pointer
        dma_tx_buf.tail = (dma_tx_buf.tail + dma_tx_buf.dma_len) % DMA_TX_BUFSIZE;
        // Restart DMA if there's more data to send
        start_dma();
    }
}

// Write to the ring buffer, return the number of bytes written
static int ring_buffer_write(ringbuffer_t *rb, uint8_t *data, uint16_t size) {
    uint16_t free_space;

    // Calculate available space in the ring buffer
    if (rb->head >= rb->tail) {
        free_space = DMA_TX_BUFSIZE - rb->head + rb->tail - 1;
    } else {
        free_space = rb->tail - rb->head - 1;
    }

    // Check if there's enough space, adjust write size if not
    if (size > free_space) {
    	size = free_space;
    }

    // No space in buffer, just return
    if (size == 0)
    	return size;

    // Check if write wraps around buffer
    if (rb->head + size > DMA_TX_BUFSIZE) {
        // Split copy in two, first up to end of the buffer, rest at beginning
    	// The rationale for doing it like this is that memcpy() is typically
    	// implemented using hand-optimized assembler code.
    	uint16_t first_chunk = DMA_TX_BUFSIZE - rb->head;
    	uint16_t second_chunk = size - first_chunk;
        memcpy(rb->buffer+rb->head, data, first_chunk);
        memcpy(rb->buffer, data+first_chunk, second_chunk);
        rb->head = second_chunk;
    } else {
        // Copy to buffer in one go...
        memcpy(&rb->buffer[rb->head], data, size);
        rb->head = (rb->head + size) % DMA_TX_BUFSIZE;
    }

    // Start DMA if it's not already running, otherwise
    // it will be started from the ISR.
    __disable_irq();
    start_dma();
    __enable_irq();

    return size;
}


int _write_r(struct _reent *r, int file, char *ptr, int len) {
	int n = 0;

	while (n < len) {
		n += ring_buffer_write(&dma_tx_buf, (uint8_t *)ptr, len);
		// For optimization it would be good to
		// wait for an interrupt here, but in our
		// application it's not necessary.
	}
    return len;
}
#else
void uart_put_char(uint8_t c) {
    USART_SendData(USART2, (uint8_t)c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)  == RESET){}
}

int _write_r(struct _reent *r, int file, char *ptr, int len) {
    int n;

    for (n = 0; n < len; n++) {
        if (ptr[n] == '\n') {
            uart_put_char('\r');
        }
        uart_put_char(ptr[n] & (uint16_t)0x01FF);
    }

    return len;
}
#endif // USE_DMA

uint8_t uart_get_char(){
    uint8_t val = 0;
    if (UART_COUNT > 0) {
        val = UART_BUFFER[UART_START_IDX++];
        UART_COUNT--;
    }
    return val;
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        UART_BUFFER[UART_END_IDX++] = (uint8_t)(USART2->RDR & 0xFF);
        if (UART_COUNT == UART_BUFFER_LENGTH-1){
            UART_START_IDX++;
        } else {
            UART_COUNT++;
        }
    }
}

void uart_clear(){
    UART_START_IDX = 0;
    UART_END_IDX = 0;
    UART_COUNT = 0;
}

uint8_t uart_get_count(){
    return UART_COUNT;
}

void uart_init(uint32_t baud) {
    setbuf(stdout, NULL); // Set stdout to disable line buffering
    setbuf(stdin,  NULL); // Set stdin  to disable line buffering

    // Enable Clocks
    RCC->AHBENR  |= RCC_AHBPeriph_GPIOA;    // Enable Clock for GPIO Bank A
    RCC->APB1ENR |= RCC_APB1Periph_USART2;  // Enable Clock for USART2

    // Connect pins to USART2
    GPIOA->AFR[2 >> 0x03] &= ~(0x0000000F << ((2 & 0x00000007) * 4)); // Clear alternate function for PA2
    GPIOA->AFR[2 >> 0x03] |=  (0x00000007 << ((2 & 0x00000007) * 4)); // Set alternate 7 function for PA2
    GPIOA->AFR[3 >> 0x03] &= ~(0x0000000F << ((3 & 0x00000007) * 4)); // Clear alternate function for PA3
    GPIOA->AFR[3 >> 0x03] |=  (0x00000007 << ((3 & 0x00000007) * 4)); // Set alternate 7 function for PA3

    // Configure pins PA2 and PA3 for 10 MHz alternate function
    GPIOA->OSPEEDR &= ~(0x00000003 << (2 * 2) | 0x00000003 << (3 * 2));    // Clear speed register
    GPIOA->OSPEEDR |=  (0x00000001 << (2 * 2) | 0x00000001 << (3 * 2));    // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
    GPIOA->OTYPER  &= ~(0x0001     << (2)     | 0x0001     << (3));        // Clear output type register
    GPIOA->OTYPER  |=  (0x0000     << (2)     | 0x0000     << (3));        // Set output type register (0x00 - Push pull, 0x01 - Open drain)
    GPIOA->MODER   &= ~(0x00000003 << (2 * 2) | 0x00000003 << (3 * 2));    // Clear mode register
    GPIOA->MODER   |=  (0x00000002 << (2 * 2) | 0x00000002 << (3 * 2));    // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    GPIOA->PUPDR   &= ~(0x00000003 << (2 * 2) | 0x00000003 << (3 * 2));    // Clear push/pull register
    GPIOA->PUPDR   |=  (0x00000001 << (2 * 2) | 0x00000001 << (3 * 2));    // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

    //Configure USART2
    USART2->CR1 &= ~0x00000001; // Disable USART2
    USART2->CR2 &= ~0x00003000; // Clear CR2 Configuration
    USART2->CR2 |=  0x00000000; // Set 1 stop bits
    USART2->CR1 &= ~(0x00001000 | 0x00000400 | 0x00000200 | 0x00000008 | 0x00000004); // Clear CR1 Configuration
    USART2->CR1 |=  0x00000000; // Set word length to 8 bits
    USART2->CR1 |=  0x00000000; // Set parity bits to none
    USART2->CR1 |=  0x00000004 | 0x00000008; // Set mode to RX and TX
    USART2->CR3 &= ~(0x00000100 | 0x00000200); // Clear CR3 Configuration
    USART2->CR3 |=  0x00000000; // Set hardware flow control to none

    uint32_t divider = 0, apbclock = 0, tmpreg = 0;
    RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus); // Get USART2 Clock frequency
    apbclock = RCC_ClocksStatus.USART2CLK_Frequency;

    if ((USART2->CR1 & 0x00008000) != 0) {
      // (divider * 10) computing in case Oversampling mode is 8 Samples
      divider = (2 * apbclock) / baud;
      tmpreg  = (2 * apbclock) % baud;
    } else {
      // (divider * 10) computing in case Oversampling mode is 16 Samples
      divider = apbclock / baud;
      tmpreg  = apbclock % baud;
    }

    if (tmpreg >=  baud / 2) {
        divider++;
    }

    if ((USART2->CR1 & 0x00008000) != 0) {
        // get the LSB of divider and shift it to the right by 1 bit
        tmpreg = (divider & (uint16_t)0x000F) >> 1;
        // update the divider value
        divider = (divider & (uint16_t)0xFFF0) | tmpreg;
    }

    USART2->BRR = (uint16_t)divider; // Configure baud rate

#if USE_DMA
    // Configure DMA
    // Enable DMA clock
    RCC->AHBENR |= RCC_AHBPeriph_DMA1;
    // DMA1 Channel 7 is mapped to USART2_TX
    uart_dma_channel = DMA1_Channel7;
    // Disable DMA channel first
    uart_dma_channel->CCR &= ~DMA_CCR_EN;
    // Set output address to USART2 TDR
    uart_dma_channel->CPAR = (uint32_t) &(USART2->TDR);
    // Data length = 0
    uart_dma_channel->CNDTR = 0;
    // Source and destination bit size = 8, increment memory,
    // DMA transfer direction memory->peripheral.
    uart_dma_channel->CCR = DMA_CCR_MINC | DMA_CCR_DIR;

    // Enable DMA transmit in USART2
    USART2->CR3 |=  USART_CR3_DMAT;

    // Initialize the DMA ring buffer to all zero
    memset(&dma_tx_buf, 0, sizeof(dma_tx_buf));

    // Enable DMA transfer complete interrupt
    NVIC_EnableIRQ(DMA1_Channel7_IRQn);
    uart_dma_channel->CCR |= DMA_CCR_TCIE;
#endif

    USART2->CR1 |= 0x00000001; // Enable USART2

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART2_IRQn);
}
