#include <ESP8266WiFi.h>                   // Wifi, MQTT, Ota, WifiManager


enum eTestPhase
{
	eTestPhase_Cycle,
	eTestPhase_White,
	eTestPhase_Warm,
	eTestPhase_RGB,
	eTestPhase_Green
} testphase=eTestPhase_Cycle;


#define TEST_DURATION (3*60)
#define BURNIN_DURATION (20*60)

void LightChangeChannels(uint8_t *channels);

void LeifRun()
{
	//if(WiFi.getMode()!=WIFI_AP) return;
	if(!WifiIsInManagerMode()) return;

	LightPowerOn();

	switch(testphase)
	{
	case eTestPhase_Cycle:
		{
			static int counter=0;
			static int clr=0;
			counter++;
			if(counter % 2 == 0)
			{

				uint8_t channels[5]={0,0,0,0,0};

				switch(clr % 6)
				{
					case 0:	channels[3]=255; break;	//white
					case 1:	channels[4]=255; break;	//warm
					case 2:	channels[0]=255; break;	//red
					case 3:	channels[1]=255; break;	//green
					case 4:	channels[2]=255; break;	//blue
				}

				LightChangeChannels(channels);

				clr++;
			}

			if(counter>TEST_DURATION && clr % 6==0)
			{
				uint8_t channels[5]={0,0,0,0,0};
				LightChangeChannels(channels);
				testphase=eTestPhase_White;
			}
			break;
		}
	case eTestPhase_White:
		{
			static int counter=0;
			
			uint8_t channels[5]={0,0,0,0,0};
			channels[3]=255; //white
			LightChangeChannels(channels);
			
			counter++;
			if(counter>BURNIN_DURATION)
			{
				uint8_t channels[5]={0,0,0,0,0};
				LightChangeChannels(channels);
				testphase=eTestPhase_Warm;
			}
			break;
		}
	case eTestPhase_Warm:
		{
			static int counter=0;
			
			uint8_t channels[5]={0,0,0,0,0};
			channels[4]=255; //warm
			LightChangeChannels(channels);
			
			counter++;
			if(counter>BURNIN_DURATION)
			{
				uint8_t channels[5]={0,0,0,0,0};
				LightChangeChannels(channels);
				testphase=eTestPhase_RGB;
			}
			break;
		}
	case eTestPhase_RGB:
		{
			static int counter=0;
			
			uint8_t channels[5]={255,255,255,0,0};	//RGB
			LightChangeChannels(channels);
			
			counter++;
			if(counter>BURNIN_DURATION)
			{
				uint8_t channels[5]={0,0,0,0,0};
				LightChangeChannels(channels);
				testphase=eTestPhase_Green;
			}
			break;
		}
	case eTestPhase_Green:
		{
			uint8_t channels[5]={0,255,0,0,0};	//Green
			LightChangeChannels(channels);
			break;
		}
	};


};
