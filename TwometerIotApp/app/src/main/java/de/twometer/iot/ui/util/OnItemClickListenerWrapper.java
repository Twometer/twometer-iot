package de.twometer.iot.ui.util;

import android.view.View;

public class OnItemClickListenerWrapper<T> {

    private OnItemClickListener<T> listener;

    public void set(OnItemClickListener<T> listener) {
        this.listener = listener;
    }

    public void bind(View view, T item) {
        view.setOnClickListener(v -> {
            if (listener != null)
                listener.onItemClicked(item);
        });
    }

}
