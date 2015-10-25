package com.ysong.colorled;

import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Toast;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    private static final String IP = "192.168.4.1";
    private static final String IP_INVALID = "0.0.0.0";
    private Toast toast = null;
    private EditText routerName = null;
    private String rName = null;
    private EditText routerPassword = null;
    private String rPassword = null;
    private Button routerLogin = null;
    private Button getIP = null;
    private SharedPreferences sp = null;
    private String ip = null;
    private DatagramSocket s = null;
    private String update = null;
    private SeekBar seekRed = null;
    private SeekBar seekGreen = null;
    private SeekBar seekBlue = null;
    private SeekBar seekHue = null;

    private void toastShow(String str) {
        toast.setText(str);
        toast.show();
    }

    private void threadToastShow(final String str) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                toastShow(str);
            }
        });
    }

    private void enableSeekBar(boolean red, boolean green, boolean blue, boolean hue) {
        seekRed.setEnabled(red);
        seekGreen.setEnabled(green);
        seekBlue.setEnabled(blue);
        seekHue.setEnabled(hue);
    }

    private void threadEnableSeekBar(final boolean red, final boolean green, final boolean blue, final boolean hue) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                enableSeekBar(red, green, blue, hue);
            }
        });
    }

    private void enableStationWidget(boolean enable) {
        routerName.setEnabled(enable);
        routerPassword.setEnabled(enable);
        routerLogin.setEnabled(enable);
        getIP.setEnabled(enable);
    }

    private void threadEnableStationWidget(final boolean enable) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                enableStationWidget(enable);
            }
        });
    }

    private String toHex(int x) {
        final char[] hex = {
                '0', '1', '2', '3',
                '4', '5', '6', '7',
                '8', '9', ':', ';',
                '<', '=', '>', '?'
        };
        return "" + hex[x / 16] + hex[x % 16];
    }

    class ConnectThread implements Runnable {
        @Override
        public void run() {
            try {
                s = new DatagramSocket();
                if (ip.equals(IP)) {
                    threadEnableStationWidget(true);
                }
                threadEnableSeekBar(true, true, true, true);
                new Thread(new updateThread()).start();
                threadToastShow("Connect UDP Success");
            } catch (Exception e) {
                threadToastShow(e.toString());
            }
        }
    }

    class routerLoginThread implements Runnable {
        @Override
        public void run() {
            try {
                byte txBuf[] = ("N" + rName + "\nP" + rPassword + "\n").getBytes();
                DatagramPacket txPkt = new DatagramPacket(txBuf, txBuf.length, InetAddress.getByName(ip), 5678);
                s.send(txPkt);
                byte rxBuf[] = new byte[32];
                DatagramPacket rxPkt = new DatagramPacket(rxBuf, rxBuf.length);
                s.receive(rxPkt);
                String str = new String(rxPkt.getData(), 0, rxPkt.getLength());
                if (str.equals("OK")) {
                    threadToastShow("Router Login Success");
                } else {
                    threadToastShow("Router Login Fail");
                }
            } catch (Exception e) {
            }
        }
    }

    class getIPThread implements Runnable {
        @Override
        public void run() {
            try {
                byte txBuf[] = "IP".getBytes();
                DatagramPacket txPkt = new DatagramPacket(txBuf, txBuf.length, InetAddress.getByName(ip), 5678);
                s.send(txPkt);
                byte rxBuf[] = new byte[32];
                DatagramPacket rxPkt = new DatagramPacket(rxBuf, rxBuf.length);
                s.receive(rxPkt);
                String str = new String(rxPkt.getData(), 0, rxPkt.getLength());
                if (str.equals("FAIL")) {
                    threadToastShow("Get Station IP Fail");
                } else {
                    SharedPreferences.Editor ed = sp.edit();
                    ed.putString("ip", str);
                    ed.commit();
                    threadToastShow("Get Station IP Success: " + str);
                }
            } catch (Exception e) {
            }
        }
    }

    class updateThread implements Runnable {
        @Override
        public void run() {
            try {
                while (s != null) {
                    if (update != null) {
                        byte txBuf[] = update.getBytes();
                        DatagramPacket txPkt = new DatagramPacket(txBuf, txBuf.length, InetAddress.getByName(ip), 5678);
                        s.send(txPkt);
                        update = null;
                    }
                    Thread.sleep(25);
                }
            } catch (Exception e) {
            }
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        toast = Toast.makeText(MainActivity.this, "", Toast.LENGTH_SHORT);
        routerName = (EditText)findViewById(R.id.router_name);
        routerPassword = (EditText)findViewById(R.id.router_password);
        routerLogin = (Button)findViewById(R.id.router_login);
        getIP = (Button)findViewById(R.id.get_ip);
        enableStationWidget(false);
        sp = getSharedPreferences("IP", MODE_PRIVATE);
        seekRed = (SeekBar)findViewById(R.id.seek_red);
        seekGreen = (SeekBar)findViewById(R.id.seek_green);
        seekBlue = (SeekBar)findViewById(R.id.seek_blue);
        seekHue = (SeekBar)findViewById(R.id.seek_hue);
        enableSeekBar(false, false, false, false);
        seekRed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progresValue, boolean fromUser) {
                update = "R" + toHex(progresValue);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                enableSeekBar(true, false, false, false);
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                enableSeekBar(true, true, true, true);
            }
        });
        seekGreen.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progresValue, boolean fromUser) {
                update = "G" + toHex(progresValue);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                enableSeekBar(false, true, false, false);
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                enableSeekBar(true, true, true, true);
            }
        });
        seekBlue.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progresValue, boolean fromUser) {
                update = "B" + toHex(progresValue);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                enableSeekBar(false, false, true, false);
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                enableSeekBar(true, true, true, true);
            }
        });
        seekHue.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progresValue, boolean fromUser) {
                update = "H" + toHex(progresValue);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                enableSeekBar(false, false, false, true);
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                enableSeekBar(true, true, true, true);
            }
        });
    }

    @Override
    protected void onStop() {
        if (s != null) {
            try {
                enableSeekBar(false, false, false, false);
                if (ip.equals(IP)) {
                    enableStationWidget(false);
                }
                s.close();
                s = null;
            } catch (Exception e) {
            }
        }
        update = null;
        rPassword = null;
        rName = null;
        ip = null;
        seekHue = null;
        seekBlue = null;
        seekGreen = null;
        seekRed = null;
        sp = null;
        getIP = null;
        routerLogin = null;
        routerPassword = null;
        routerName = null;
        toast = null;
        super.onStop();
    }

    public void connectAPHandler(View view) {
        if (s == null) {
            ip = IP;
            new Thread(new ConnectThread()).start();
        } else {
            toastShow("Connect UDP Already");
        }
    }

    public void connectStationHandler(View view) {
        if (s == null) {
            ip = sp.getString("ip", IP_INVALID);
            if (ip.equals(IP_INVALID)) {
                toastShow("Invalid IP");
            } else {
                new Thread(new ConnectThread()).start();
            }
        } else {
            toastShow("Connect UDP Already");
        }
    }

    public void routerLoginHandler(View view) {
        rName = routerName.getText().toString();
        rPassword = routerPassword.getText().toString();
        if (rName.equals("") || rPassword.equals("")) {
            toastShow("Field Empty");
        } else {
            new Thread(new routerLoginThread()).start();
        }
    }

    public void getIPHandler(View view) {
        new Thread(new getIPThread()).start();
    }

    public void disconnectHandler(View view) {
        if (s == null) {
            toastShow("Disconnect Already");
        } else {
            try {
                enableSeekBar(false, false, false, false);
                if (ip.equals(IP)) {
                    enableStationWidget(false);
                }
                s.close();
                s = null;
                toastShow("Disconnect Success");
            } catch (Exception e) {
                toastShow(e.toString());
            }
        }
    }
}
