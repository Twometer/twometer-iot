package de.twometer.iot.ui.bridge;

import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import de.twometer.iot.R;

class InfoItemViewHolder extends RecyclerView.ViewHolder {

    private TextView headerLabel;
    private TextView contentLabel;

    InfoItemViewHolder(@NonNull View itemView) {
        super(itemView);
        headerLabel = itemView.findViewById(R.id.header_label);
        contentLabel = itemView.findViewById(R.id.content_label);
    }

    void configure(InfoItem item) {
        headerLabel.setText(item.getHeaderRes());
        contentLabel.setText(item.getValue());
    }

}
