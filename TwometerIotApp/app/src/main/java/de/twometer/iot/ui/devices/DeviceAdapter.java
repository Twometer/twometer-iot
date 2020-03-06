package de.twometer.iot.ui.devices;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

import de.twometer.iot.R;
import de.twometer.iot.model.Device;

public class DeviceAdapter extends RecyclerView.Adapter<DeviceViewHolder> {

    private List<Device> dataset;

    @NonNull
    @Override
    public DeviceViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        LayoutInflater inflater = LayoutInflater.from(parent.getContext());
        View rootView = inflater.inflate(R.layout.item_device, parent, false);
        return new DeviceViewHolder(rootView);
    }

    @Override
    public void onBindViewHolder(@NonNull DeviceViewHolder holder, int position) {
        Device device = dataset.get(position);
        if (device != null)
            holder.configure(device);
    }

    @Override
    public int getItemCount() {
        return dataset != null ? dataset.size() : 0;
    }

    void setDataset(List<Device> dataset) {
        this.dataset = dataset;
        notifyDataSetChanged();
    }

}
