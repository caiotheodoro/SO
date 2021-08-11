
public class ResourceMonitor {
    private String buffer;

    private boolean empty = true;

    public synchronized String take() {
        // Wait until buffer is available.
        while (empty) {
            try {
                wait();
            } catch (InterruptedException e) {}
        }
        // Toggle status.
        empty = true;
        // Notify producer that status has changed.
        notifyAll();
        return buffer;
    }

    public synchronized void put(String buffer) {
        // Wait until buffer has been retrieved.
        while (!empty) {
            try { 
                wait();
            } catch (InterruptedException e) {}
        }
        // Toggle status.
        empty = false;
        // Store buffer.
        this.buffer = buffer;
        // Notify consumer that status has changed.
        notifyAll();
    }
}

