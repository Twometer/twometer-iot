package de.twometer.iot.handler;

import de.twometer.iot.alexa.model.ErrorType;
import de.twometer.iot.alexa.response.ErrorResponse;
import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.alexa.response.SceneResponse;
import de.twometer.iot.handler.base.IHandler;
import de.twometer.iot.net.BridgeClient;
import de.twometer.iot.scene.Scene;
import de.twometer.iot.scene.SceneAction;
import de.twometer.iot.scene.SceneManager;

import java.util.List;

public class SceneHandler implements IHandler {
    @Override
    public IResponse handle(Request request, BridgeClient client, SceneManager ext) {
        boolean activate = request.getAction().equals("Activate");
        String sceneId = request.getEndpoint();

        Scene scene = ext.findScene(sceneId);
        if (scene == null)
            return new ErrorResponse(request, ErrorType.NO_SUCH_ENDPOINT, "Scene does not exist");

        if (!scene.isCanSwitchOff() && !activate)
            return new ErrorResponse(request, ErrorType.INVALID_DIRECTIVE, "Scene does not support deactivation");

        List<SceneAction> actions = activate ? scene.getActivation() : scene.getDeactivation();

        for (SceneAction action : actions) {
            client.setProperty(action.getDeviceId(), action.getPropertyId(), action.getValue());
        }

        String responseName = activate ? "ActivationStarted" : "DeactivationStarted";
        return new SceneResponse(request.getCorrelationToken(), sceneId, responseName);
    }

    @Override
    public String getNamespace() {
        return "Alexa.SceneController";
    }
}
