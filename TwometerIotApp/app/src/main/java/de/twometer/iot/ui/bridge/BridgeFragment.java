package de.twometer.iot.ui.bridge;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout;

import java.util.ArrayList;
import java.util.List;

import de.twometer.iot.R;

public class BridgeFragment extends Fragment {

    private BridgeViewModel bridgeViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        bridgeViewModel = new ViewModelProvider(this).get(BridgeViewModel.class);
        View root = inflater.inflate(R.layout.fragment_bridge, container, false);
        final RecyclerView recyclerView = root.findViewById(R.id.bridge_details_list);
        final SwipeRefreshLayout refreshLayout = root.findViewById(R.id.bridge_details_refresher);
        final InfoItemAdapter adapter = new InfoItemAdapter();
        recyclerView.setAdapter(adapter);
        recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));

        bridgeViewModel.getBridgeData().observe(getViewLifecycleOwner(), debugInfo -> {
            refreshLayout.setRefreshing(false);
            List<InfoItem> items = new ArrayList<>();
            items.add(new InfoItem(R.string.item_name, debugInfo.getName()));
            items.add(new InfoItem(R.string.item_version, debugInfo.getVersion()));
            items.add(new InfoItem(R.string.item_registry_tokens, debugInfo.getRegistryTokenCount()));
            items.add(new InfoItem(R.string.item_connected_devices, debugInfo.getConnectedDeviceCount()));
            items.add(new InfoItem(R.string.item_registered_devices, debugInfo.getRegisteredDeviceCount()));
            items.add(new InfoItem(R.string.item_mode, debugInfo.getMode()));
            items.add(new InfoItem(R.string.item_wifi_ssid, debugInfo.getSsid()));
            items.add(new InfoItem(R.string.item_wifi_rssi, debugInfo.getRssi()));
            items.add(new InfoItem(R.string.item_sys_time, debugInfo.getSystemTime()));
            items.add(new InfoItem(R.string.item_last_ping, debugInfo.getLastPingTime()));
            items.add(new InfoItem(R.string.item_pair_exit_req, String.valueOf(debugInfo.isPairShutdownScheduled())));
            adapter.setDataset(items);
        });
        refreshLayout.setOnRefreshListener(() -> bridgeViewModel.refresh());
        return root;
    }
}