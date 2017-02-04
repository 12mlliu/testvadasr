package com.midea.VadAsr;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;


import com.midea.VadAsr.R;
import com.midea.VadAsr.R.id;
import com.midea.VadAsr.R.layout;
import com.midea.VadAsr.Config;




public class MainActivity extends Activity implements OnClickListener {

	private Button mButtonStart = null;
	private Button mButtonStop = null;

	static {
		System.loadLibrary("pocketsphinx_jni");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mButtonStart = (Button) this.findViewById(R.id.btn_start);
		mButtonStop = (Button) this.findViewById(R.id.btn_stop);
		mButtonStart.setOnClickListener(this);

	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.btn_start:
			onStartRecord();
			break;
		default:
			break;
		}
	}
	private void onStartRecord() {
		//Config c = Decoder.defaultConfig();
		Config c  = new Config();
        c.setString("-hmm", "/mnt/sdcard/lml/cn-model/tdt_sc_8k");
        c.setString("-lm", "/mnt/sdcard/lml/cn-model/ask.lm.DMP");
        c.setString("-dict", "/mnt/sdcard/lml/cn-model/ask.dic");
        c.setFloat("-samprate",8000);
        
        //Decoder d = new Decoder(c);
		

        @SuppressWarnings("resource")
		FileInputStream ais = null;
		try {
			ais = new FileInputStream(new File("/mnt/sdcard/lml/cn-model/test.raw"));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//Decoder d = new Decoder();
        //d.startUtt();
        //d.setRawdataSize(300000);
        /*byte[] b = new byte[4096];
        int nbytes;
        try {
			while ((nbytes = ais.read(b)) >= 0) {
			    ByteBuffer bb = ByteBuffer.wrap(b, 0, nbytes);
			    bb.order(ByteOrder.LITTLE_ENDIAN);
			    short[] s = new short[nbytes/2];
			    bb.asShortBuffer().get(s);
			    //System.out.println(s);
			    //d.processRaw(s, nbytes/2, false, false);
			    PocketSphinx.decoderTest(c,s,nbytes);
			    System.out.println(PocketSphinx.decoderTest(c,s,nbytes));
			    
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
		byte[] b = new byte[49152];
        int nbytes;
        try {
			nbytes = ais.read(b);
			ByteBuffer bb = ByteBuffer.wrap(b, 0, nbytes);
			bb.order(ByteOrder.LITTLE_ENDIAN);
			short[] s = new short[nbytes/2];
			bb.asShortBuffer().get(s);
			    //System.out.println(s);
			    //d.processRaw(s, nbytes/2, false, false);
			PocketSphinx.decoderTest(c,s,nbytes);
			System.out.println(PocketSphinx.decoderTest(c,s,nbytes));
			    
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        //d.endUtt();
        //System.out.println("+++INFO: " + d.hyp().getHypstr());
	
	}
}
