///////++++++++++++++Complicated Method++++++++++++++++++++++////////////////  

uint8_t bin[4];
uint8_t n;
uint8_t i;

uint8_t a[10]; 

void binaryId()
{
	dec[0] = 0;
	
	if(ID1){
	 bin[0] = 0;
	}else{
	 bin[0] = 1;
	}
	if(ID2){
	 bin[1] = 0;
	}else{
	 bin[1] = 1;
	}
	if(ID3){
	 bin[2] = 0;
	}else{
	 bin[2] = 1;
	}
	if(ID4){
	 bin[3] = 0;
	}else{
	 bin[3] = 1;
	}
	
HAL_Delay(100);	
	n = bin[3] + 10* bin[2] + 100*bin[1] + 1000*bin[0] ;
	
HAL_Delay(100);	
	
	for(i=0;n!=0;i++)
	{
		a[i]=n%10;
		n=n/10;
	}
	n=i;
	for(i=0;i<n;i++)
	{
		dec[0]+=a[i]*pow(2,i);
	}
HAL_Delay(100);
}

////////////+++++++++++++++++++///////////////



void id_detect()
{
	if(ID1 == SET){
		slave_id = slave_id | 0x01;
	}
	if(ID2 == SET){
		slave_id = slave_id | 0x02;
	}
	if(ID3 == SET){
		slave_id = slave_id | 0x04;
	}
	if(ID4 == SET){
		slave_id = slave_id | 0x08;
	}
}

