package de.twometer.iot.net.discovery;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.Objects;

import de.twometer.iot.model.Bridge;

public class BridgeDiscovery {

    private static final byte[] DISCOVERY_PAYLOAD = {0x00, 0x42, 0x69};

    private static final int DISCOVERY_PORT = 38711;

    public static Bridge discover(Context context) throws IOException {
        InetAddress broadcastAddress = getBroadcastAddress(context);
        InetSocketAddress address = new InetSocketAddress(broadcastAddress, DISCOVERY_PORT);

        DatagramSocket socket = new DatagramSocket();
        socket.setSoTimeout(25000);
        socket.setBroadcast(true);
        socket.send(new DatagramPacket(DISCOVERY_PAYLOAD, DISCOVERY_PAYLOAD.length, address));

        byte[] data = new byte[128];
        DatagramPacket reply = new DatagramPacket(data, data.length);
        socket.receive(reply);

        String ip = new String(data).trim();
        return new Bridge(ip, "", "");
    }

    private static InetAddress getBroadcastAddress(Context context) throws IOException {
        WifiManager wifi = (WifiManager) context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        DhcpInfo dhcp = Objects.requireNonNull(wifi).getDhcpInfo();
        if (dhcp == null)
            return null;
        // handle null somehow

        int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
        byte[] quads = new byte[4];
        for (int k = 0; k < 4; k++)
            quads[k] = (byte) (broadcast >> (k * 8));
        return InetAddress.getByAddress(quads);
    }

}
