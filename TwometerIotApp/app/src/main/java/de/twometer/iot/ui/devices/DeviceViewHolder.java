package de.twometer.iot.ui.devices;

import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import de.twometer.iot.R;
import de.twometer.iot.model.Device;

class DeviceViewHolder extends RecyclerView.ViewHolder {

    private TextView deviceNameLabel;
    private ImageView deviceImage;

    DeviceViewHolder(@NonNull View itemView) {
        super(itemView);
        deviceNameLabel = itemView.findViewById(R.id.device_name_label);
        deviceImage = itemView.findViewById(R.id.device_image);
    }

    void configure(Device device) {
        deviceNameLabel.setText(device.getPreferredName());
        deviceImage.setImageResource(device.getType().getImageRes());
    }

}
