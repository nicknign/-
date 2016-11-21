package wangye.bluetooth;


import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Iterator;
import java.util.Set;
import java.util.UUID;



import android.media.ExifInterface;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.annotation.TargetApi;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.Toast;
import android.widget.TextView;

@TargetApi(Build.VERSION_CODES.GINGERBREAD)
public class MainActivity extends Activity {
	
	private static final String TAG = "TAG-AndroidCameraActivity";     
    public static final int MEDIA_TYPE_IMAGE = 1;   
    public static final int MEDIA_TYPE_VIDEO = 2;
    public static final int MAXSPEED = 100;
	private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	private BluetoothAdapter mBluetoothAdapter = null;
	private BluetoothSocket btSocket = null;
	private OutputStream outStream = null;
	private String address;
	//private WifiManager wifi_manager=null;
	
	private Camera mCamera; 
    private CameraPreview mPreview;
    private boolean previewstate;
    private int speed;
    
	Button mButtonF;	   
    Button mButtonB;
    Button mButtonL;
    Button mButtonR;
    Button mButtonS;
   // Button wifi_start;
   // Button wifi_stop;
   // Button wifi_read;
    Button btnSend;
    Button link;    
    Button CameraTest;
    Button btnspeedup;
    Button btnspeeddown;
    TextView speedshown;
    TextView tvShowCon;
    EditText etReceive, etIp, etPort;
    server1 s;
    String message;
    byte[] msgBuffer;
    
    private void initView() {
    	tvShowCon = (TextView) findViewById(R.id.tvshown);
    	etReceive = (EditText) findViewById(R.id.receive);
    	link = (Button) findViewById(R.id.link);
		etIp = (EditText) findViewById(R.id.editip);
		etPort = (EditText) findViewById(R.id.port);
		btnSend = (Button) findViewById(R.id.test);
		mButtonF=(Button)findViewById(R.id.btnF);
		mButtonB=(Button)findViewById(R.id.btnB);
		mButtonL=(Button)findViewById(R.id.btnL);
		mButtonR=(Button)findViewById(R.id.btnR);		
		CameraTest=(Button)findViewById(R.id.cameratest);
		btnspeedup=(Button)findViewById(R.id.speedup);
		btnspeeddown=(Button)findViewById(R.id.speeddown);
		speedshown=(TextView)findViewById(R.id.speedshow);
    }
    private void initListener() {
    	link.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				String ip = etIp.getText().toString().trim();
				System.out.println("IP ----->"+ip);
				String port = etPort.getText().toString().trim();
				System.out.println("port ----->"+port);
				int p = 0;
				if (port.length()>0) {
					p = Integer.parseInt(port);
				}
				s = new server1(handler, ip, p);
				s.start();
			}
		});    	
    	CameraTest.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub				
				if(previewstate){					
			        mCamera.takePicture(null, null, mPicture);			        
			        previewstate=false;		            
				}
				else{					
					mCamera.startPreview();
		            previewstate=true;		        
				}            
			}
    		
    	});
    	
    	btnspeedup.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(speed<MAXSPEED)
						speed++; 
				message=String.valueOf(speed);
				speedshown.setText(message);
				speedshown.invalidate();
			}
    		
    	});
    	
    	btnspeeddown.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(speed>0)
						speed--;  
				message=String.valueOf(speed);
				speedshown.setText(message);
				speedshown.invalidate();
			}
    		
    	});
    	
    	btnSend.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				String msg = "mobile link test";
				printLog("msg:" + msg);
				s.sendMsg(msg);
			}
		});
    	mButtonF.setOnTouchListener(new Button.OnTouchListener(){
    	       
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub				
				int action = event.getAction();
				switch(action)
				{
				case MotionEvent.ACTION_DOWN:
					qianjin();
					break;				
				case MotionEvent.ACTION_UP:
					tingzhi();
					break;
				}
				return false;
			}
    	 });
    	mButtonB.setOnTouchListener(new Button.OnTouchListener(){		
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub				
				int action = event.getAction();
				switch(action)
				{
				case MotionEvent.ACTION_DOWN:
				houtui();
				break;
				
				case MotionEvent.ACTION_UP:
					tingzhi();
					break;
				}				
				return false;
			}    
        });
    	mButtonL.setOnTouchListener(new Button.OnTouchListener(){
        	
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub				
				int action = event.getAction();
				switch(action)
				{
				case MotionEvent.ACTION_DOWN:
				zuozhuan();
				break;
				
				case MotionEvent.ACTION_UP:
					tingzhi();
					break;
				}
				
				return false;
				
             }
        });
    	mButtonR.setOnTouchListener(new Button.OnTouchListener(){
            
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub				
				int action = event.getAction();
				switch(action)
				{
				case MotionEvent.ACTION_DOWN:
				youzhuan();
				break;
				
				case MotionEvent.ACTION_UP:
					tingzhi();
					break;
				}				
				return false;			
			}    
        });
    }
    
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);	
		System.out.println("onCreat ---->");
		previewstate=false;
		speed=35;
		
		initView();
		initListener();  
		
		mCamera = getCameraInstance();	
		mPreview = new CameraPreview(this, mCamera); 
		//setCameraDisplayOrientation(this,0,mCamera);
        final FrameLayout preview = (FrameLayout) findViewById(R.id.camera_preview1); 
        preview.addView(mPreview);
        previewstate=true;
      //������������
      	//	wifi_start=(Button)findViewById(R.id.startwifi);      		
      	//	wifi_start.setOnClickListener(new View.OnClickListener() {
      		 
      	//	public void onClick(View v) {
      		 
      	//		wifi_manager=(WifiManager)MainActivity.this.getSystemService(Context.WIFI_SERVICE);
      	//		wifi_manager.setWifiEnabled(true);
      			
      	//		DisplayToast("wifi״̬Ϊ" + wifi_manager.getWifiState());
      		 
      	//	}
      		 
      	//	});
        
      	//�ر���������
      	//	wifi_stop=(Button)findViewById(R.id.stopwifi);      		
      	//	wifi_stop.setOnClickListener(new View.OnClickListener() {
      		 
      	//	public void onClick(View v) {
      		 
      	//		wifi_manager=(WifiManager)MainActivity.this.getSystemService(Context.WIFI_SERVICE);
      	//		wifi_manager.setWifiEnabled(false);
      	//		System.out.println("wifi_state ---->" + wifi_manager.getWifiState());
      	//		DisplayToast("wifi״̬Ϊ" + wifi_manager.getWifiState());
      		 
      	//	}
      		 
      	//	});
      		
      	//��ѯ��������
      	//	wifi_read=(Button)findViewById(R.id.readwifi);      		
      	//	wifi_read.setOnClickListener(new View.OnClickListener() {
      		 
      	//	public void onClick(View v) {    		 
      		
      	//		System.out.println("wifi_state ---->" + wifi_manager.getWifiState());
      	//		DisplayToast("wifi״̬Ϊ" + wifi_manager.getWifiState());
      		 
      	//	}
      		 
      	//	});    
      //�ж���������
      		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
      		if(mBluetoothAdapter != null){
      			System.out.println("����ӵ������");
      			if(!mBluetoothAdapter.isEnabled()){
      				Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
      				startActivity(intent);
      				while(!mBluetoothAdapter.isEnabled())
						try {
							Thread.currentThread().sleep(200);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
      				}
      			Set<BluetoothDevice> devices = mBluetoothAdapter.getBondedDevices();
      				if(devices.size()>0){
      					for(Iterator iterator = devices.iterator();iterator.hasNext();){
	      					BluetoothDevice bluetoothDevice = (BluetoothDevice) iterator.next();
	      					System.out.println(bluetoothDevice.getAddress());
	      					address = bluetoothDevice.getAddress();
	      					System.out.println("������ַ"+address);
	      					} 
      				}
      				else{
      					System.out.println("û����������豸");
      				}
      			}
      			else{
      				System.out.println("û�������豸");
      			}	
	}
	
	

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		System.out.println("onResume-------------------->");
		System.out.println("start link the client");
		BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
		try {

            btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);

         } catch (IOException e) {

              DisplayToast("�׽��ִ���ʧ�ܣ�");
         }
        // DisplayToast("�ɹ���������С�������Կ�ʼ�ٿ���~~~");	
         mBluetoothAdapter.cancelDiscovery();
         try {

             btSocket.connect();
             DisplayToast("���ӳɹ��������������Ӵ򿪣�");
         } catch (IOException e) {

             try {
             	btSocket.close();

             } catch (IOException e2) {
            	 
                     DisplayToast("����û�н������޷��ر��׽��֣�");
             }
         }
         System.out.println("the link is done");
		
	}
	
	Handler handler = new Handler() {
		public void handleMessage(android.os.Message msg) {
			switch (msg.what) {
			case 0:
				tvShowCon.setText("���ӳɹ�");
				tvShowCon.invalidate();
				break;

			case 1:
				String str = (String) msg.obj;
				printLog("���յ�����Ϣ:" + str);
				System.out.println("���յ�����Ϣ:" + str);
				etReceive.setText(str);				
				if(isNumeric(str))
				{
					int i;
					i=Integer.parseInt(str);
					speed=i%100;
					System.out.println("speed="+speed);
					AnalyseCommand(str);
				}
				else if(isT(str))
				{
					System.out.println("Ttakepicture");
					if(previewstate){					
				        mCamera.takePicture(null, null, mPicture);			        
				        previewstate=false;		            
					}
					else{					
						mCamera.startPreview();
			            previewstate=true;
					}
				}
				etReceive.invalidate();
				break;
			case 2:
				tvShowCon.setText("�������ӵ�������,���Ե�...");
				tvShowCon.invalidate();
				break;
			}

		};
	};
	
	private void AnalyseCommand(String str){
		int i;
		i=Integer.parseInt(str);
		i=i/100;
		switch(i){
		case 0:
			tingzhi();
			break;
		case 1:
			qianjin();
			break;
		case 2:
			zuozhuan();
			break;
		case 3:
			houtui();
			break;
		case 4:
			youzhuan();
			break;		
		}
	}
	
	private void tingzhi(){
		
		System.out.println("����ң�أ�0");
		try {
          	outStream = btSocket.getOutputStream();

          } catch (IOException e) {
        	  System.out.println("ON RESUME: Output stream creation failed.");
          }


          message = "0"+speed;

          msgBuffer = message.getBytes();

          try {
          	outStream.write(msgBuffer);
          	System.out.println("write "+message);

          } catch (IOException e) {
        	  System.out.println("ON RESUME: Exception during write."); 
          }
	}
	private void qianjin(){
		
		System.out.println("����ң�أ�1");
		try {
          	outStream = btSocket.getOutputStream();

          } catch (IOException e) {
              System.out.println("ON RESUME: Output stream creation failed.");
          }


          message = "1"+speed;

         msgBuffer = message.getBytes();

          try {
          	outStream.write(msgBuffer);
          	System.out.println("write "+message);

          } catch (IOException e) {
        	  System.out.println("ON RESUME: Exception during write.");              
        	}
          
       /*   try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
          message = "0"+speed;

          msgBuffer = message.getBytes();

           try {
           	outStream.write(msgBuffer);
           	System.out.println("write "+message);

           } catch (IOException e) {
         	  System.out.println("ON RESUME: Exception during write.");              
         	}*/
          
	}
	private void zuozhuan(){
		
		System.out.println("����ң�أ�2");
		try {
          	outStream = btSocket.getOutputStream();

          } catch (IOException e) {
        	  System.out.println("ON RESUME: Output stream creation failed.");
          }


          message = "2"+speed;

         msgBuffer = message.getBytes();

          try {
          	outStream.write(msgBuffer);
          	System.out.println("write "+message);
          } catch (IOException e) {
        	  System.out.println("ON RESUME: Exception during write."); 
          }
          
       /*   try {
  			Thread.sleep(500);
  		} catch (InterruptedException e) {
  			// TODO Auto-generated catch block
  			e.printStackTrace();
  		}
            message = "0"+speed;

            msgBuffer = message.getBytes();

             try {
             	outStream.write(msgBuffer);
             	System.out.println("write "+message);

             } catch (IOException e) {
           	  System.out.println("ON RESUME: Exception during write.");              
           	}*/
	}
	private void houtui(){
		
		System.out.println("����ң�أ�3");
		try {
          	outStream = btSocket.getOutputStream();

          } catch (IOException e) {
        	  System.out.println("ON RESUME: Output stream creation failed.");
          }


          message = "3"+speed;

         msgBuffer = message.getBytes();

          try {
          	outStream.write(msgBuffer);
          	System.out.println("write "+message);
          } catch (IOException e) {
        	  System.out.println("ON RESUME: Exception during write."); 
          }
          
       /*   try {
  			Thread.sleep(500);
  		} catch (InterruptedException e) {
  			// TODO Auto-generated catch block
  			e.printStackTrace();
  		}
            message = "0"+speed;

            msgBuffer = message.getBytes();

             try {
             	outStream.write(msgBuffer);
             	System.out.println("write "+message);

             } catch (IOException e) {
           	  System.out.println("ON RESUME: Exception during write.");              
           	}*/
	}
	private void youzhuan(){
		
		System.out.println("����ң�أ�4");
		try {
          	outStream = btSocket.getOutputStream();

          } catch (IOException e) {
        	  System.out.println("ON RESUME: Output stream creation failed.");
          }


          message = "4"+speed;

         msgBuffer = message.getBytes();

          try {
          	outStream.write(msgBuffer);
          	System.out.println("write "+message);
          } catch (IOException e) {
        	  System.out.println("ON RESUME: Exception during write."); 
          }
          
       /*   try {
  			Thread.sleep(500);
  		} catch (InterruptedException e) {
  			// TODO Auto-generated catch block
  			e.printStackTrace();
  		}
            message = "0"+speed;

            msgBuffer = message.getBytes();

             try {
             	outStream.write(msgBuffer);
             	System.out.println("write "+message);

             } catch (IOException e) {
           	  System.out.println("ON RESUME: Exception during write.");              
           	}*/
	}
	
	public static boolean isNumeric(String str){
		   for(int i=str.length();--i>=0;){
		      int chr=str.charAt(0);
		      if(chr<48 || chr>57)
		         return false;
		   }
		   return true;
		}
	public static boolean isT(String str){
		
		char[] strChar = str.toCharArray();
		for(int i=str.length();--i>=0;){
		      if(strChar[i]=='T')
		         return true;
		   }
		   return false;
		}


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	public void DisplayToast(String str)
    {
    	Toast toast=Toast.makeText(this, str, Toast.LENGTH_LONG);
    	toast.setGravity(Gravity.TOP, 0, 220);
    	toast.show();
    	
    }
	private void printLog(String s) {
		Log.i("info", s);
	}
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		System.out.println("onPause ---->");
		DisplayToast("�����˳�");
		this.onDestroy();
	}
	
	/** ��ȫ��ȡCamera����ʵ���ķ���*/ 
	public static Camera getCameraInstance(){ 
	    Camera c = null; 
	    try { 
	        c = Camera.open(); // ��ͼ��ȡCameraʵ��
	    } 
	    catch (Exception e){ 
	        // ����ͷ�����ã�����ռ�û򲻴��ڣ�
	    } 
	    return c; // �������򷵻�null
	}
	
	private PictureCallback mPicture = new PictureCallback() { 
		 
	    @Override 
	    public void onPictureTaken(byte[] data, Camera camera) { 
	    	
	        File pictureFile = getOutputMediaFile(MEDIA_TYPE_IMAGE); 
	        if (pictureFile == null){ 
	        	System.out.println("Error creating media file, check storage permissions: "); 
	            return; 
	        }          
	    	try{//���ͼƬ  
	    			Bitmap bMap;
	    		    bMap = BitmapFactory.decodeByteArray(data, 0,data.length);        
	    		    Bitmap bMapRotate;  
	    		    Configuration config = getResources().getConfiguration();
	    		    if (config.orientation==1)
	    		    { // ����
	    		    	Matrix matrix = new Matrix();
	    		    	matrix.reset();
	    		    	matrix.postRotate(0);
	    		    	bMapRotate = Bitmap.createBitmap(bMap, 0, 0,bMap.getWidth(), bMap.getHeight(),matrix, true);
	    		    	bMap = bMapRotate;
	    		    }
	    		    //	
	    		    FileOutputStream fos = new FileOutputStream(pictureFile);
	    		    BufferedOutputStream bos = new BufferedOutputStream(fos); 	    		    
	    		    bMap.compress(Bitmap.CompressFormat.JPEG,40,bos);//��ͼƬѹ�������� 
	    		    bos.flush();//��� 
	    		    bos.close();//�ر� 	    		    
	    		    fos.close();		    
	    		
	          //����ʱ�ɹر���һ��
		        s.sendFile("/sdcard/Pictures/MyCameraApp/IMG_temppic.jpg");
		        
	        } catch (FileNotFoundException e) { 
	            Log.d(TAG, "File not found: " + e.getMessage()); 
	        } catch (IOException e) { 
	            Log.d(TAG, "Error accessing file: " + e.getMessage()); 
	        } 
	        
	        //data����
	        //s.sendData(data,data.length);
	        
	        if(previewstate){					
		        mCamera.takePicture(null, null, mPicture);
		        previewstate=false;		            
			}
			else{					
				mCamera.startPreview();
	            previewstate=true;		        
			} 
	    }		 
	};
	
	
	/** Ϊ����ͼƬ����Ƶ����File */ 
	private static File getOutputMediaFile(int type){ 
	    // ��ȫ�������ʹ��ǰӦ��
	    // ��Environment.getExternalStorageState()���SD���Ƿ���װ��
	 
	    File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory( 
	              Environment.DIRECTORY_PICTURES), "MyCameraApp"); 
	// �������ͼƬ��Ӧ�ó���ж�غ󻹴��ڡ����ܱ�����Ӧ�ó�����
	// ��˱���λ�������
	 
	    // ��������ڵĻ����򴴽��洢Ŀ¼
	    if (! mediaStorageDir.exists()){ 
	        if (! mediaStorageDir.mkdirs()){ 
	            Log.d("MyCameraApp", "failed to create directory"); 
	            return null; 
	        } 
	    } 
	    
	    // ����ý���ļ���
	    //String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date()); 
	    String timeStamp="temppic";
	    File mediaFile; 
	    if (type == MEDIA_TYPE_IMAGE){ 
	        mediaFile = new File(mediaStorageDir.getPath() + File.separator + 
	        "IMG_"+ timeStamp + ".jpg"); 
	    } else if(type == MEDIA_TYPE_VIDEO) { 
	        mediaFile = new File(mediaStorageDir.getPath() + File.separator + 
	        "VID_"+ timeStamp + ".mp4"); 
	    } else { 
	        return null; 
	    } 
	 
	    return mediaFile; 
	}
	
	/** ����������ͷԤ���� */ 
	public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback { 	   
		private SurfaceHolder mHolder; 
	    private Camera mCamera; 
	 
	    public CameraPreview(Context context, Camera camera) { 
	        super(context); 
	        mCamera = camera; 
	 
	        // ��װһ��SurfaceHolder.Callback��
	        // �������������ٵײ�surfaceʱ�ܹ����֪ͨ��
	        mHolder = getHolder(); 
	        mHolder.addCallback(this); 
	        // �ѹ��ڵ����ã����汾����3.0��Android����Ҫ
	       // mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS); 
	    } 
	 
	    public void surfaceCreated(SurfaceHolder holder) { 
	        // surface�ѱ����������ڰ�Ԥ�������λ��֪ͨ����ͷ
	        try { 
	            mCamera.setPreviewDisplay(holder); 
	            mCamera.startPreview(); 
	            System.out.println("surface created");
	            
	        } catch (IOException e) { 
	            Log.d(TAG, "Error setting camera preview: " + e.getMessage()); 
	           
	        } 
	    } 
	 
	    public void surfaceDestroyed(SurfaceHolder holder) { 
	        // �մ��롣ע����activity���ͷ�����ͷԤ������  	    	
	    	releaseCamera();
	    	System.out.println("surface destroyed");
	    	
	    	} 
	 
	    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) { 
	        // ���Ԥ���޷����Ļ���ת��ע��˴����¼�
	        // ȷ�������Ż�����ʱֹͣԤ��
	 
	        if (mHolder.getSurface() == null){ 
	          // Ԥ��surface������
	          return; 
	        } 
	 
	        // ����ʱֹͣԤ�� 
	        try { 
	            mCamera.stopPreview(); 
	        } catch (Exception e){ 
	          // ���ԣ���ͼֹͣ�����ڵ�Ԥ��
	        } 
	 
	        // �ڴ˽������š���ת��������֯��ʽ
	 
	        // ���µ���������Ԥ��
	        try { 
	        	//��������������
	        	Camera.Parameters parameters = mCamera.getParameters();
	        	//����ͼƬ����ʱ�ķֱ��ʴ�С
	        	parameters.setPictureSize(640, 384);	
	        	
	        	float[] distances = new float[3];   
	        	System.out.println("Focus Mode: "+ parameters.getFocusMode());
	        	 
	        	
	        	parameters.getFocusDistances(distances);
	        	
	        	System.out.println("focus distance 0"+ Float.toString(distances[0]));
	        	
	        	 
	        	
	        	System.out.println("focus distance 1"+ Float.toString(distances[1]));
	        	
	        	 
	        	
	        	System.out.println("focus distance 2"+ Float.toString(distances[2]));
	        	
	        Display display = ((WindowManager)getSystemService(WINDOW_SERVICE)).getDefaultDisplay(); 
	        	 
	            if(display.getRotation() == Surface.ROTATION_0) 
	            { 	                                            
	                mCamera.setDisplayOrientation(90); 
	            } 
	     
	            if(display.getRotation() == Surface.ROTATION_90) 
	            { 
	                                           
	            } 
	     
	            if(display.getRotation() == Surface.ROTATION_180) 
	            { 
	                                
	            } 
	     
	            if(display.getRotation() == Surface.ROTATION_270) 
	            { 	                 
	                mCamera.setDisplayOrientation(180); 
	            } 
	        	
	        	mCamera.setParameters(parameters);
	            mCamera.setPreviewDisplay(mHolder); 
	            mCamera.startPreview(); 
	            System.out.println("surface changed");
	        } catch (Exception e){ 
	            Log.d(TAG, "Error starting camera preview: " + e.getMessage()); 
	            
	        } 
	    }
	    
	}
	
	
	private void releaseCamera(){ 

        if (mCamera != null){ 

            mCamera.release();        // Ϊ����Ӧ���ͷ�����ͷ

            mCamera = null; 

        } 

    } 
	
	public static int getDisplayRotation(Activity activity) {  
	    int rotation = activity.getWindowManager().getDefaultDisplay()  
	       .getRotation();  
	    switch (rotation) {  
	        case Surface.ROTATION_0: return 0;  
	        case Surface.ROTATION_90: return 90;  
	        case Surface.ROTATION_180: return 180;  
	        case Surface.ROTATION_270: return 270;  
	    }  
	    return 0;  
	}  
	  
	public static void setCameraDisplayOrientation(Activity activity,  
	        int cameraId, Camera camera) {  
	    // See android.hardware.Camera.setCameraDisplayOrientation for  
	    // documentation.  
	    Camera.CameraInfo info = new Camera.CameraInfo();  
	    Camera.getCameraInfo(cameraId, info);  
	    int degrees = getDisplayRotation(activity);  
	    int result;  
	    if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {  
	        result = (info.orientation + degrees) % 360;  
	        result = (360 - result) % 360;  // compensate the mirror  
	    } else {  // back-facing  
	        result = (info.orientation - degrees + 360) % 360;  
	    }  
	    camera.setDisplayOrientation(result);  
	}  
	
}
