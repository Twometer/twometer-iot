package de.twometer.iot.scene;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class SceneManager {

    private final ObjectMapper mapper = new ObjectMapper();
    private final File workingDirectory = Paths.get("").toAbsolutePath().toFile();
    private final File scenesFile = new File(workingDirectory, "scenes.json");
    private List<Scene> scenes = new ArrayList<>();

    public SceneManager() {
        load();
    }

    private void load() {
        if (!scenesFile.exists())
            return;

        try {
            scenes = mapper.readValue(scenesFile, this.getClass()).getScenes();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void deleteScene(String id) {
        for (Scene s : scenes)
            if (s.getId().equals(id)) {
                scenes.remove(s);
                return;
            }
    }

    public void save() {
        try {
            mapper.writeValue(scenesFile, this);
        } catch (IOException e) {
            System.err.println("Processing failed");
            e.printStackTrace();
        }
    }

    public void addScene(Scene scene) {
        scenes.add(scene);
    }

    public Scene findScene(String id) {
        for (Scene s : scenes)
            if (s.getId().equals(id))
                return s;
        return null;
    }

    public List<Scene> getScenes() {
        return scenes;
    }

}
