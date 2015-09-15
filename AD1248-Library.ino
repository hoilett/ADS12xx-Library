#include <SPI.h>
#include "ads12xx.h"

const int  START = 14;
const int  CS = 10;
const int  DRDY = 15;


ads12xx ADS(CS, START, DRDY);  //initialize ADS as object of the ads1248 class
ads12xx::regValues_t Voltage;


void setup()
{


	Serial.begin(115200);
	Serial.println("Serial online");
	
	

	ADS.Reset();
	delay(10);
#ifdef ADS1248



	Voltage.SYS0_val = DOR3_2000 | PGA2_0;
	
	Voltage.IDAC0_val = DRDY_ONLY | IMAG2_OFF | IDAC0_ID;
	Voltage.IDAC1_val = I1DIR_OFF;
	Voltage.MUX0_val = BCS1_1|  MUX_SP2_AIN0 | MUX_SN2_AIN1;
	Voltage.VBIAS_val = VBIAS_OFF;
#endif

//	ADS.calibration(SYSGCAL);
	//ADS.calibration(SELFOCAL);

	

	
	

}

void loop(){
	//test_intTemp();
	//test_supVoltage();
	//test_Voltage();
	//test_extrefVoltage();
	//test_Thermo();
	
	//ADS.Reset();
	
	delay(25);
	

}


#ifdef ADS1248

void test_intTemp(){
	
	Voltage.MUX1_val = MUXCAL2_TEMP | VREFCON1_ON | REFSELT1_ON;  
	ADS.SetRegister(Voltage);
	unsigned long temp_val = ADS.readSingle(Voltage);
	Serial.println(temp_val, DEC);
	double temp_voltage = (2.048 / 8388608)*temp_val;	//hier nur die 23Bit genommen
	//Serial.println(temp_voltage, DEC);
	double temp = ((temp_voltage-0.118) / (405 * 0.000001))+25;	//Siehe Datasheet S.30			
	//Serial.print("Internal Temperature is ");
	//Serial.print(temp);
	//Serial.println(" Grad Celsius!");
	
}


void test_supVoltage(){
	
	Voltage.MUX1_val = MUXCAL2_DVDD | VREFCON1_ON | REFSELT1_ON;
	ADS.SetRegister(Voltage);
	unsigned long volt_val = ADS.readSingle(Voltage);
	//Serial.println(volt_val, DEC);
	double voltage = (2.048 / 16777216)*volt_val;
	voltage *= 4*2.048;
	Serial.print("DVDD Voltage: ");
	Serial.print(voltage);
	Serial.println("V");
	
	Voltage.MUX1_val = MUXCAL2_AVDD | VREFCON1_ON | REFSELT1_ON;
	ADS.SetRegister(Voltage);
	unsigned long volt1_val = ADS.readSingle(Voltage);
	//Serial.println(volt1_val, DEC);
	double voltage1 = (2.048 / 16777216)*volt1_val;
	voltage1 *= 4 * 2.048;
	Serial.print("AVDD Voltage: ");
	Serial.print(voltage1);
	Serial.println("V");
	
}

void test_extrefVoltage(){

	Voltage.MUX1_val = REFSELT1_ON | VREFCON1_ON | MUXCAL2_REF1;	  //ADS Reference on Intern, Internal Reference on, System Montitor on REF1
	Voltage.IDAC0_val = IMAG2_1500;			 //	IDAC at 1,5mA current
	Voltage.IDAC1_val = I1DIR_AIN0 | I2DIR_OFF;			 // IDAC1 Currentoutput on AIN0, IDAC2 off
	Voltage.SYS0_val = DOR3_5;
	ADS.SetRegister(Voltage);
	unsigned long volt_val = ADS.readSingle(Voltage);
	Serial.println(volt_val, DEC);
	double voltage = (2.048 / (16777216))*volt_val;
	voltage *= 4 * 2.048;
	//Serial.print("External V_Ref: ");
	//Serial.print(voltage, DEC);
	//Serial.println("V");


}


void test_Voltage(){
	Voltage.MUX0_val = MUX_SN2_AIN4 | MUX_SP2_AIN5;
	Voltage.MUX1_val = REFSELT1_REF1 | VREFCON1_ON;	  //ADS Reference on Intern, Internal Reference on
	Voltage.IDAC0_val = IMAG2_1500;			 //	IDAC at 1,5mA current
	Voltage.IDAC1_val = I1DIR_AIN1;			 // IDAC Currentsink AIN1
	Voltage.SYS0_val = PGA2_8 | DOR3_320;
	ADS.SetRegister(Voltage);
	
	unsigned long volt_val = ADS.readSingle(Voltage);
	Serial.println(volt_val, DEC);
	
	double voltage = (1.54 /(16777216/2))*volt_val;
	double ohm = voltage / (0.0015*8);
	//Serial.print("Resistance:");
	//Serial.print(ohm,DEC);
	//Serial.println("Ohm");


}


void test_Thermo(){
	Voltage.MUX0_val = MUX_SP2_AIN6 | MUX_SN2_AIN7;
	Voltage.MUX1_val = REFSELT1_ON| VREFCON1_ON;	  //ADS Reference on Intern, Internal Reference on
	Voltage.VBIAS_val = VBIAS_7;
	Voltage.SYS0_val = PGA2_128 ;		   // 2000 sps vollkommen un�tz rauschen �berwiegt
	ADS.SetRegister(Voltage);
	long volt_val = ADS.readSingle(Voltage);
	if (long minus = volt_val >> 23 == 1){
		long volt_valneg = volt_val - 16777216;
		Serial.println(volt_valneg, DEC);
	}
	else
	{
		Serial.println(volt_val, DEC);
	}


	

	//Serial.println(minus, BIN);
	
	
	double voltage = (2.048 / (16777216*2))*(volt_val/32);
	//Serial.print("Thermocouple: ");
	//Serial.println(voltage, DEC);
}

#endif


