# DE Enable pin for Modbus transmittion 


void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  if(__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_TC)){
		HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_SET);
	}
	else if(__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE) == 0){
		HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_RESET);
	}
  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

