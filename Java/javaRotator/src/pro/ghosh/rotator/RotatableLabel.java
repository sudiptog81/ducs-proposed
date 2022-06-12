package pro.ghosh.rotator;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.File;

public class RotatableLabel extends JLabel {
    double angle = 0.;
    BufferedImage image;

    RotatableLabel() {
        super();
        loadImage("D:\\compass.png");
    }

    void loadImage(String path) {
        try {
            this.image = ImageIO.read(new File(path));
        } catch (Exception ignored) {

        }
    }

    public double getAngle() {
        return this.angle;
    }

    public void setAngle(double angle) {
        this.angle = angle;
    }

    @Override
    public void paintComponent(Graphics g) {
        AffineTransform at = AffineTransform.getTranslateInstance(13, 20);
        at.rotate(
                Math.toRadians(this.angle),
                getX() + this.image.getWidth() / 4.,
                getY() + this.image.getHeight() / 4.
        );
        at.scale(.5, .5);
        Graphics2D gx = (Graphics2D) g;
        gx.drawImage(this.image, at, null);
        super.paintComponent(g);
        repaint();
    }
}
