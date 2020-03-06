package de.twometer.iot.ui.devices;

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

import de.twometer.iot.R;

public class DevicesFragment extends Fragment {

    private DevicesViewModel devicesViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        devicesViewModel = new ViewModelProvider(this).get(DevicesViewModel.class);
        View root = inflater.inflate(R.layout.fragment_devices, container, false);

        final RecyclerView recyclerView = root.findViewById(R.id.device_list);
        final SwipeRefreshLayout refreshLayout = root.findViewById(R.id.device_list_refresher);
        final DeviceAdapter adapter = new DeviceAdapter();
        recyclerView.setAdapter(adapter);
        recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));

        devicesViewModel.getDevices().observe(getViewLifecycleOwner(), devices -> {
            refreshLayout.setRefreshing(false);
            adapter.setDataset(devices);
        });

        refreshLayout.setOnRefreshListener(() -> devicesViewModel.refresh());

        return root;
    }
}