package de.twometer.iot;

import java.util.function.Consumer;

public class Promise<T> {

    private Runnable errorHandler;

    private Consumer<T> consumer;

    private T result;

    private final Object waitLock = new Object();

    public void resolve(T t) {
        this.result = t;
        synchronized (waitLock) {
            waitLock.notifyAll();
        }

        if (consumer != null)
            consumer.accept(t);
    }

    public void reject() {
        this.result = null;
        synchronized (waitLock) {
            waitLock.notifyAll();
        }

        if (errorHandler != null)
            errorHandler.run();
    }

    public T await() {
        synchronized (waitLock) {
            try {
                waitLock.wait(30000);
            } catch (InterruptedException e) {
                e.printStackTrace();
                return null;
            }

            return result;
        }
    }

    public void then(Consumer<T> consumer) {
        this.consumer = consumer;
    }

}
