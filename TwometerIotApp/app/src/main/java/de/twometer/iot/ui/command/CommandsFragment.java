package de.twometer.iot.ui.command;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import de.twometer.iot.R;

public class CommandsFragment extends Fragment {

    private CommandsViewModel commandsViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        commandsViewModel = new ViewModelProvider(this).get(CommandsViewModel.class);
        View root = inflater.inflate(R.layout.fragment_commands, container, false);
        final TextView textView = root.findViewById(R.id.text_slideshow);
        commandsViewModel.getText().observe(getViewLifecycleOwner(), textView::setText);
        return root;
    }
}