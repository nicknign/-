package wangye.bluetooth;

import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class server1 extends Thread {
	Handler handler;
	PrintWriter out;
	OutputStream outputStream;
	String ip;
	int port;

	public server1(Handler handler, String ip, int port) {
		this.handler = handler;
		this.ip = ip;
		this.port = port;
	}

	@Override
	public void run() {
		try {
			Log.i("info", "服务正在开启...");
			handler.sendEmptyMessage(2);
			Socket socket = null;			
			if (ip == null || "".equals(ip) || port == 0) {
				System.out.println("link to -->"+"192.168.22.11"+" port:6800");
				socket = new Socket("192.168.22.11", 6800);
			} else {
				System.out.println("link to -->"+ip+" port:"+port);
				socket = new Socket(ip, port);
			}
			Log.i("info", "服务开启成功...");
			handler.sendEmptyMessage(0);
			InputStream in = socket.getInputStream();
			 InputStreamReader reader = new InputStreamReader(in, "gbk");
//			out = new PrintWriter(socket.getOutputStream(), true);
			 outputStream = socket.getOutputStream();
			 out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(
						socket.getOutputStream(), "gbk")), true);
			// 一直读取服务器端发来的消息
			char[] c;
			int a;
			String receiverMsg = "";
			while (true) {
				c = new char[30];
				while((a = reader.read(c,0,c.length))!=-1){
					receiverMsg= new String(c,0,a);
					Log.i("info", "receiverMsg:" + receiverMsg);
					Message msg = Message.obtain();
					msg.obj = receiverMsg;
					msg.what = 1;
					handler.sendMessage(msg);
				}
				c = null; 
			}
			// while (true) {
			// // 可读的最大长度
			// try {
			// length = in.available();
			// if (length <= 0) {
			// continue;
			// }
			// b = new byte[5];
			// while ((a = in.read(b, 0, b.length)) != -1) {
			// receiverMsg += new String(b, 0, a);
			// Log.i("info", "receiverMsg:" + receiverMsg);
			// Message msg = Message.obtain();
			// msg.obj = receiverMsg;
			// msg.what = 1;
			// handler.sendMessage(msg);
			// }
			//
			// // off = b.length;
			// } catch (IOException e) {
			// // TODO Auto-generated catch block
			// e.printStackTrace();
			// }

			// }
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void sendMsg(String msg) {
		out.println(msg);
	}
	
	public void sendFile(String FilePath) {
		System.out.println("connected!reading...");
		InputStream inputStream = null;
		try {
			inputStream = new FileInputStream(FilePath);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		System.out.println("sending...");
		

		byte buffer [] =new byte[200*1024];
		int temp = 0;
		try {
			int lenth = inputStream.available();
			String s;
			s = lenth+"";
			int l = s.length();
			for(int i=0;i<6-l;i++)
				s=s+" ";
			out.println(s);
			
			System.out.println(s);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}		
		try {
			while((temp = inputStream.read(buffer)) != -1){
				outputStream.write(buffer,0,temp);				
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("sending complete!");
		try {
			outputStream.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	public void sendData(byte[] data,int length)
	{
		out.println(length);
		System.out.println(length);
		try {
			sleep(10);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			outputStream.write(data);
			outputStream.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}

}
