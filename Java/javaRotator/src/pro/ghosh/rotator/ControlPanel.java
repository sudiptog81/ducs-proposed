package pro.ghosh.rotator;

import javax.swing.*;

public class ControlPanel {
    JFrame frame;
    JButton btnIncSpeed, btnDecSpeed;
    static ControlPanel controlPanel;

    public static void createInstance() {
        if (controlPanel == null)
            controlPanel = new ControlPanel();
    }

    ControlPanel() {
        this.frame = new JFrame("Control Panel");
        this.frame.setResizable(false);
        this.frame.setVisible(true);
        this.frame.setLayout(null);
        this.frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        this.frame.setSize(300, 100);
        this.frame.setLocation(0, 410);

        this.btnIncSpeed = new JButton("Speed +");
        this.btnDecSpeed = new JButton("Speed -");
        this.btnIncSpeed.setBounds(30, 15, 100, 30);
        this.btnDecSpeed.setBounds(155, 15, 100, 30);
        this.btnIncSpeed.addActionListener(e -> Main.logic.setSpeed(Main.logic.getSpeed() + 10));
        this.btnDecSpeed.addActionListener(e -> Main.logic.setSpeed(Main.logic.getSpeed() - 10));
        this.frame.add(this.btnIncSpeed);
        this.frame.add(this.btnDecSpeed);
    }
}
