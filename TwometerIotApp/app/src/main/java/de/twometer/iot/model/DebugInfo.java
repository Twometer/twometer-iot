package de.twometer.iot.model;

import org.jetbrains.annotations.NotNull;

public class DebugInfo {

    private String name;
    private String version;
    private int registryTokenCount;
    private int connectedDeviceCount;
    private int registeredDeviceCount;
    private String mode;
    private String ssid;
    private int rssi;
    private int systemTime;
    private int lastPingTime;
    private boolean pairShutdownScheduled;

    public DebugInfo(String name, String version, int registryTokenCount, int connectedDeviceCount, int registeredDeviceCount, String mode, String ssid, int rssi, int systemTime, int lastPingTime, boolean pairShutdownScheduled) {
        this.name = name;
        this.version = version;
        this.registryTokenCount = registryTokenCount;
        this.connectedDeviceCount = connectedDeviceCount;
        this.registeredDeviceCount = registeredDeviceCount;
        this.mode = mode;
        this.ssid = ssid;
        this.rssi = rssi;
        this.systemTime = systemTime;
        this.lastPingTime = lastPingTime;
        this.pairShutdownScheduled = pairShutdownScheduled;
    }

    public String getName() {
        return name;
    }

    public String getVersion() {
        return version;
    }

    public int getRegistryTokenCount() {
        return registryTokenCount;
    }

    public int getConnectedDeviceCount() {
        return connectedDeviceCount;
    }

    public int getRegisteredDeviceCount() {
        return registeredDeviceCount;
    }

    public String getMode() {
        return mode;
    }

    public String getSsid() {
        return ssid;
    }

    public int getRssi() {
        return rssi;
    }

    public int getSystemTime() {
        return systemTime;
    }

    public int getLastPingTime() {
        return lastPingTime;
    }

    public boolean isPairShutdownScheduled() {
        return pairShutdownScheduled;
    }

    @NotNull
    @Override
    public String toString() {
        return "DebugInfo{" +
                "name='" + name + '\'' +
                ", version='" + version + '\'' +
                ", registryTokenCount=" + registryTokenCount +
                ", connectedDeviceCount=" + connectedDeviceCount +
                ", registeredDeviceCount=" + registeredDeviceCount +
                ", mode='" + mode + '\'' +
                ", ssid='" + ssid + '\'' +
                ", rssi=" + rssi +
                ", systemTime=" + systemTime +
                ", lastPingTime=" + lastPingTime +
                ", pairShutdownScheduled=" + pairShutdownScheduled +
                '}';
    }
}
