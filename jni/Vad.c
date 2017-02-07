/*
 * VoiceDetect.c
 *
 *  Created on: Feb 5, 2017
 *      Author: mlliu 
 */



#include"Vad.h"

//short buf_data[VAD_DETECT_FRAMELAST][DATASAPLES];
//int   mVadFlagBuf[VAD_DETECT_FRAMELAST];
//float mEnvEnergyBuf[ENV_DETECT_FRAMELAST];
//std::vector<short>svec;
//short VadData[20000];

//bool isGetEnvEnergy = false; // is get enviroment energy

bool isVadStaFlagSucceed=false;
bool isVadStaFlag=false;
bool isVadEndFlag=false;
int count=0;
int num=0;
float vadEnergyTht =  0.0; // vad energy tht
int Datasize=0;

//byte2short
/* extern short byte2short(byte low, byte high)
{
		// TODO Auto-generated method stub
		return (short) (((high & 0x00FF) << 8) | (0x00FF & low));
}*/
//
 int getWaveTimeLast(int nSamples, int fs)
	{
		float last = 0;
		//last = ((float) nSamples / fs) * 1000;
		last = ((float) nSamples / fs);

		return (int) (last * 1000);
	}
 	// get energy
	float getFrameEnergy(int16 const *data, int len)
   {
		float energy = (float) 0.0;
		for (int i = 0; i < len; i++)
		{
			energy += data[i] * data[i] / (float) len;
		}
		energy = (float)sqrt(energy);
		return energy;
	}
	// get Frame Energy Mean
	 float getEnergyMean(float *energyArray, int frameCnt)
	 {
		float mean = 0;
		for (int i = 0; i < frameCnt; i++) {
			mean += energyArray[i];
		}
		mean = mean / frameCnt;
		return mean;
	}
	// init value for next vad process
/*	 void mideaVadReset()
	{
		isVadSta = false;
		isVadEnd = false;
	}*/
// get enviroment energy
    bool getEnvEnergy(float *envEnergyDur, int frameCnt,float *envEnergyBase)
	//bool getEnvEnergy(float *envEnergyDur, int frameCnt,float *envEnergyBase)
	{
		int flag = 0;
		int temp = 0;
		for (int i = 0; i < frameCnt - 1; i++) {
			if (abs(envEnergyDur[i + 1] - envEnergyDur[i]) < ENV_ENERGY_MINVAR) {
				temp = 0;
			} else {
				temp = 1;
			}
			flag += temp;
		}
		if (0 == flag) {
			//isGetEnvEnergy = true;
			// envEnergyBase = envEnergyDur[frameCnt - 2];
			*envEnergyBase = getEnergyMean(envEnergyDur, frameCnt);
			vadEnergyTht = VAD_TRIGGER_RATIO * (*envEnergyBase);
		}

		//return isGetEnvEnergy;
		return true;
	}
	// get vad flag
    int getVadFlag(float energy,float vadEnergyTht)
	{
		//if (isGetEnvEnergy) {
			if (energy > (vadEnergyTht)) {
				return 1;
			} else {
				return 0;
			}
		//} else {
			//return -1;
		//}
	}
	// is vad start
	 bool isVadStartPointDetect(int* vadArray, int frameCnt) {
		if (frameCnt < VAD_STA_FRAMELAST_THT) {
			return false;
		}
		int staFlag = 1; // ��ʼΪ1
		int staTemp = 0;
		int cnt = 0;
		for (int i = 0; i < frameCnt - 1; i++) {

			if (vadArray[i + 1] == 1 && vadArray[i] == 0) {
				staTemp = 1;
			}
			if (1 == staTemp) {
				cnt++;
				staFlag &= vadArray[i + 1];
				if (cnt >= VAD_STA_FRAMELAST_THT) {
					if (1 == staFlag) {
						return true;
					} else {
						return false;
					}
				}
			}
		}

		return false;
	}

	// is vad end
	bool isVadFinishPointDetect(int *vadArray, int frameCnt)
	{
		if (frameCnt < VAD_STA_FRAMELAST_THT)
		{
			return false;
		}
		int endFlag = 0;
		int endTemp = 0;
		int cnt = 0;
		for (int i = 0; i < frameCnt - 1; i++)
		{
			if (vadArray[i + 1] == 0 && vadArray[i] == 1)
			{
				endTemp = 1;
			}
			if (1 == endTemp) {
				cnt++;
				endFlag |= vadArray[i + 1];
				if (cnt >= VAD_END_FRAMELAST_THT)
				{
					if (0 == endFlag) {
						return true;
					} else {
						return false;
					}
				}
			}
		}

		return false;
	}

	char const * VoiceDetectMain(int16 const *mData,cmd_ln_t *config)
	{
		int mFrameLastVad=VAD_DETECT_FRAMELAST;//6
		float mEnergy = 0;
		int mVadFlag = 0;
		char const*no_hyp = "false";
		char const*hyp;


		printf("+++INFO: start voice activity detect!");
		mEnergy=getFrameEnergy(mData, DATASAPLES);
		printf("+++INFO: mEnergy[%d] = %f\n" ,num,mEnergy);
		mVadFlag = getVadFlag(mEnergy,vadEnergyTht);
		// System.out.println("+++INFO: mVadFlag = " + mVadFlag);
		if (num < mFrameLastVad)
		{
			mVadFlagBuf[num] = mVadFlag;
			for(int i=0;i<DATASAPLES;i++)
			buf_data[num][i]=mData[i];
			num++;
			return no_hyp;
		}
		else
		{
			for (int i = 1; i < mFrameLastVad; i++)
			{
				mVadFlagBuf[i - 1] = mVadFlagBuf[i];
			}
			mVadFlagBuf[mFrameLastVad - 1] = mVadFlag;

			for(int k=1;k<mFrameLastVad;k++)
			{
				for(int i=0;i<DATASAPLES;i++)
					{
						buf_data[k-1][i]=buf_data[k][i];
					}
			}
			for(int i=0;i<DATASAPLES;i++)
				{
					buf_data[mFrameLastVad-1][i]=mData[i];
				}


			if (false == isVadStaFlagSucceed)
			{
				isVadStaFlag = isVadStartPointDetect(mVadFlagBuf,
						mFrameLastVad);
				if (isVadStaFlag)
				{
					printf("+++INFO: Detected start point!");
					// save data,push the buf_data to the svec
					for (int i = 0; i < mFrameLastVad-1; i++)
					{
						for(int j=0;j<DATASAPLES;j++)
						{
							//svec.push_back(buf_data[i][j]);
							VadData[j+i*DATASAPLES]=buf_data[i][j];

						}
					}
					Datasize = mFrameLastVad-1;
				}
				else
					return no_hyp;
			}

			if (isVadStaFlag)
			{
				isVadStaFlagSucceed = true;
				// save data
				for(int j=0;j<DATASAPLES;j++)
					{
						//svec.push_back(mData[j]);
					    VadData[j+ Datasize * DATASAPLES]=mData[j];

					}
				Datasize++;
				isVadEndFlag = isVadFinishPointDetect(mVadFlagBuf, mFrameLastVad);
				if (isVadEndFlag)
				{
					//after isVadEndFlag==true,all the flag must return to be false
					//isEnvEnergyDetect=false;
		            count = 0;
		            num = 0;
		            //Datasize=0;
					isVadStaFlag = false;
					isVadEndFlag = false;
					isVadStaFlagSucceed = false;

					printf("+++INFO: Detected end point!");

					int waveLast = getWaveTimeLast(Datasize * DATASAPLES, 8000);//fs=8000;
					if ((waveLast > VALID_WAVE_LAST_MIN)&& (waveLast < VALID_WAVE_LAST_MAX))
					{
							printf("+++INFO: The Voice is Valid for ASR!");

							printf("+++INFO: The Voice size is %d: ",Datasize * DATASAPLES);
							hyp = ps_decoder_test(config,VadData,Datasize* DATASAPLES);
							/*
							printf("+++INFO: hyp = %s\n",hyp);
  							FILE *outfile;
    							outfile = fopen("result.txt","at");
							fputs(hyp,outfile);
                					fputs("\n",outfile);
							fclose(outfile);*/

							return hyp;
					}
					else
					{
						    printf("+++INFO: The Voice size is %d: ",Datasize * DATASAPLES);
							printf("+++INFO: The Voice is Unvalid for ASR!");
							return no_hyp;
					}
				}
				else
					return no_hyp;
				}
			}
	}


	bool EnvEnergyDetect(int16 const *mData)
	{
		 int mFrameLastEnv=ENV_DETECT_FRAMELAST;
		 float mEnergy = 0;
	     float envEnergyBase =  0.0; // envEnergy

		 mEnergy=getFrameEnergy(mData, DATASAPLES);

		 if ((count) < mFrameLastEnv)
		 {
				mEnvEnergyBuf[count] = mEnergy;
				printf("+++INFO: mEnergy[%d] = %f\n" ,count,mEnergy);
				count ++;

				return false;
		 }
		 else
		 {
				for (int i = 1; i < mFrameLastEnv; i++)
					{
						mEnvEnergyBuf[i - 1] = mEnvEnergyBuf[i];
					}
					mEnvEnergyBuf[mFrameLastEnv - 1] = mEnergy;
					if (getEnvEnergy(mEnvEnergyBuf, mFrameLastEnv,&envEnergyBase))
					{
						printf("+++INFO: Enviroment Energy obtain success!");
						printf("+++INFO: Env Energy = %f\n",envEnergyBase);

						return true;
					}
				}
		}




/*char const *ps_decoder_test(cmd_ln_t *config, int16 *data,long total)
{
    ps_decoder_t *ps;
    char const *hyp;
    int32 score;

    ps = ps_init(config);
    ps_start_utt(ps);
    //Test it first with pocketsphinx_decode_raw()

    ps_process_raw(ps, data, total, false, true);
    ps_end_utt(ps);

    hyp = ps_get_hyp(ps, &score);

    return hyp;
}*/




