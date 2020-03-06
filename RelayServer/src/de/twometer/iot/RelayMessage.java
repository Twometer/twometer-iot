package de.twometer.iot;

import java.util.UUID;

public class RelayMessage {

    private String content;

    private Promise<String> responsePromise;

    public RelayMessage(String content, Promise<String> responsePromise) {
        this.content = content;
        this.responsePromise = responsePromise;
    }

    public String getContent() {
        return content;
    }

    public Promise<String> getResponsePromise() {
        return responsePromise;
    }
}
