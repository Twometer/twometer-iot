package de.twometer.iot.ui.bridge;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

import de.twometer.iot.R;

public class InfoItemAdapter extends RecyclerView.Adapter<InfoItemViewHolder> {

    private List<InfoItem> dataset;

    @NonNull
    @Override
    public InfoItemViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        LayoutInflater inflater = LayoutInflater.from(parent.getContext());
        View rootView = inflater.inflate(R.layout.item_debug_info, parent, false);
        return new InfoItemViewHolder(rootView);
    }

    @Override
    public void onBindViewHolder(@NonNull InfoItemViewHolder holder, int position) {
        InfoItem infoItem = dataset.get(position);
        if (infoItem != null)
            holder.configure(infoItem);
    }

    @Override
    public int getItemCount() {
        return dataset != null ? dataset.size() : 0;
    }

    void setDataset(List<InfoItem> dataset) {
        this.dataset = dataset;
        notifyDataSetChanged();
    }
}
