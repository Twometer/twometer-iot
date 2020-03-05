package de.twometer.iot.ui.bridge;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import de.twometer.iot.R;

public class BridgeFragment extends Fragment {

    private BridgeViewModel bridgeViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        bridgeViewModel = new ViewModelProvider(this).get(BridgeViewModel.class);
        View root = inflater.inflate(R.layout.fragment_bridge, container, false);
        final TextView textView = root.findViewById(R.id.text_tools);
        bridgeViewModel.getBridgeData().observe(getViewLifecycleOwner(), debugInfo -> {

        });
        return root;
    }
}