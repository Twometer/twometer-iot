package de.twometer.iot.scene;

import java.util.ArrayList;
import java.util.List;

public class Scene {

    private final List<SceneAction> activation = new ArrayList<>();
    private final List<SceneAction> deactivation = new ArrayList<>();
    private String id;
    private String friendlyName;
    private String description;
    private boolean canSwitchOff;

    public Scene(String friendlyName, String description, boolean canSwitchOff) {
        this.friendlyName = friendlyName;
        this.description = description;
        this.canSwitchOff = canSwitchOff;
    }

    public List<SceneAction> getActivation() {
        return activation;
    }

    public List<SceneAction> getDeactivation() {
        return deactivation;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getFriendlyName() {
        return friendlyName;
    }

    public void setFriendlyName(String friendlyName) {
        this.friendlyName = friendlyName;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public boolean isCanSwitchOff() {
        return canSwitchOff;
    }

    public void setCanSwitchOff(boolean canSwitchOff) {
        this.canSwitchOff = canSwitchOff;
    }
}
