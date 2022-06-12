package pro.ghosh.rotator;

public class RotatorLogic implements Runnable {
    double speed = 0;

    public void setSpeed(double speed) {
        this.speed = speed;
    }

    public double getSpeed() {
        return speed;
    }

    public void run()
    {
        try {
            while (true) {
                Main.lblRotator.setAngle((Main.lblRotator.getAngle() + speed) % 360);
                Thread.sleep(20);
            }
        }
        catch (Exception ignored) {

        }
    }
}
