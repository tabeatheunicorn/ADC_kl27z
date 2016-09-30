#include "MKL27Z644.h"

void Delay()
{
	int i;
	for (i = 0; i < 1000000; i++)
		asm("nop");
}

int ergebnis = 0;
//ADC aktivieren, um Spannung (Poti) anzuzeigen
void initADC()
{
	ADC0_SC3 |= ADC_SC3_CAL(1);
	ADC0_SC1A |= ADC_SC1_DIFF(0)|ADC_SC1_ADCH(1); // Input channel select, should enable the adc, doesnt
	//set clock
	SIM_SCGC6 |= SIM_SCGC6_ADC0(1);
	SIM_SCGC5 |= SIM_SCGC5_PORTE(1);
	SIM_SOPT2 |= SIM_SOPT2_USBSRC(1);//USB clock is selected
	SIM_SCGC4 |= SIM_SCGC4_VREF(1); //VREF clock gate control enabled
	ADC0_CLPD; //calibration function, enables adc, doesnt not reading memory
	
	//voher hier adco_sc1a
	//Trigger für die conversion festlegen, welcher ist geeignet?
	SIM_SOPT7 |= SIM_SOPT7_ADC0PRETRGSEL(0); //next conversion will be stored in the A register
	SIM_SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN(1); // Alternate trigger enable, trigger comes form a pheripheral event
	
	//differentialmode disabled
	ADC0_SC1A |= ADC_SC1_DIFF(0);
	ADC0_SC1A |= ADC_SC1_AIEN(0);// converison complete interrupt disabled
	// Setzen der Einstellungen für configuration register 1
	ADC0_CFG1 |= ADC_CFG1_ADLPC(0); //Low Power Config -> normal power configuration
	ADC0_CFG1 |= ADC_CFG1_ADIV(0); // Clock Divide Select ( divide ratio 1, clock rate is input clock)
	ADC0_CFG1 |= ADC_CFG1_ADLSMP(0); //SAmple Time Configuration short sample time
	ADC0_CFG1 |= ADC_CFG1_MODE(3); // Conversion mode select (single-ended 16-bit)
	ADC0_CFG1 |= ADC_CFG1_ADICLK(0); //Input clock select (bus clock)

	ADC0_CFG2 |= ADC_CFG2_MUXSEL(0);//Channels ADxxa are selected
	ADC0_CFG2 |= ADC_CFG2_ADACKEN(0);//Asynchronous clock output disabled, enabled if selected by adiclk and conversion is active
	ADC0_CFG2 |= ADC_CFG2_ADHSC(0);// HIgh speed config longest sample time 24 adck cycles total
	//ADC0_CFG2 |= ADC_CFG2_ADLSTS(0);//Long sample time, 24 adck cycles total, sollte laut 24.5.4.5 nicht gesetzt werden
	
	// D0 bis D8 im Data register enthalten meine Wert

	ADC0_SC2 |= ADC_SC2_ADTRG(0); //Softwaretrigger shall be used
	ADC0_SC2 |= ADC_SC2_ADTRG(0)| ADC_SC2_ACFE(0) |ADC_SC2_ACFGT(0)| ADC_SC2_ACREN(0)| ADC_SC2_DMAEN(0)| ADC_SC2_REFSEL(0);
	ADC0_SC3 |= ADC_SC3_ADCO(0) | ADC_SC3_AVGE(0) | ADC_SC3_AVGS(0);

	ADC0_SC2 |= ADC_SC2_REFSEL(0); //Refernzspannung sind vref,h und vref,l
	

	
}

//Calibration function as described in the manual p 406ff, überflüssig da calibration sequence exisitiert? sc3, field 7
void calibrationFunction()
{
	int result = 0;
	//result = ADC0_CLP0 + ADC0_CLP1 + ADC0_CLP2 + ADC0_CLP3 + ADC0_CLP4 + ADC0_CLPS;
	result = ADC0_RA + ADC0_RB;
	result = result / 2;
	result |= 1 << 15;
	ADC0_PG = result;
	}

int main()
{
	initADC(); 
	//ADC0_CLPD|= adc; //calibration function
	for (;;)
	{
		
		/*ADC0_SC1A |= ADC_SC1_DIFF(0)|ADC_SC1_ADCH(1); //Input channel select
		//ADC0_SC1A |= ADC_SC1_AIEN(1);
		ADC0_SC3 |= ADC_SC3_CAL(1);
		ergebnis = ADC0_RA;
		//In Software Trigger mode, when sc2[adtrg]=0, writes to SC1A subsequently initiate a new conversion => new conversion started?
		ADC0_SC2 |= ADC_SC2_ADTRG(0);
		
		
		//Calibration seuquence begins when adc0_sc3_cal(7) is set; calf has to be checked befor		ADC0_SC3 |= ADC_SC3_CAL(1);
		*/
		
	}
	return 0;
}