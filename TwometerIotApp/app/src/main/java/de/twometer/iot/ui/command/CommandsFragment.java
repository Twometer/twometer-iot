package de.twometer.iot.ui.command;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;

import de.twometer.iot.R;

public class CommandsFragment extends Fragment {

    private CommandsViewModel commandsViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        commandsViewModel = new ViewModelProvider(this).get(CommandsViewModel.class);
        View root = inflater.inflate(R.layout.fragment_commands, container, false);
        final TextView textView = root.findViewById(R.id.text_slideshow);
        commandsViewModel.getText().observe(getViewLifecycleOwner(), new Observer<String>() {
            @Override
            public void onChanged(@Nullable String s) {
                textView.setText(s);
            }
        });
        return root;
    }
}