package de.twometer.iot.ui.devices;

import android.app.AlertDialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout;

import de.twometer.iot.IoTContext;
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

        adapter.setOnItemClickListener(device -> {

            View view = LayoutInflater.from(getContext()).inflate(R.layout.dialog_edit_device, null);
            EditText text = view.findViewById(R.id.device_name_editor);
            ImageView imageView = view.findViewById(R.id.device_image);

            text.setText(device.getPreferredName());
            imageView.setImageResource(device.getType().getImageRes());

            new AlertDialog.Builder(getContext())
                    .setTitle(R.string.action_edit_device)
                    .setView(view)
                    .setPositiveButton(R.string.ok, (dialog, which) -> {
                        String newVal = text.getText().toString();
                        if (newVal.equals(device.getPreferredName()))
                            return;

                        IoTContext.getInstance().getBackgroundHandler().post(() -> {
                            IoTContext.getInstance().getClient().renameDevice(device.getUuid(), newVal);
                            devicesViewModel.refresh();
                        });
                    })
                    .setNegativeButton(R.string.cancel, null)
                    .show();
        });

        refreshLayout.setOnRefreshListener(() -> devicesViewModel.refresh());

        return root;
    }
}