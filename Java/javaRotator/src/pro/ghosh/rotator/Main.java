package pro.ghosh.rotator;

import javax.swing.*;

public class Main {
    static JFrame frame;
    static RotatableLabel lblRotator;
    static RotatorLogic logic;

    JButton btnCtrlPanel;

    Main() {
        frame = new JFrame("Rotator");
        frame.setResizable(false);
        frame.setVisible(true);
        frame.setLayout(null);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setSize(300, 400);

        lblRotator = new RotatableLabel();
        lblRotator.setBounds(0, 0, 300, 300);
        frame.add(lblRotator);

        this.btnCtrlPanel = new JButton("Open Control Panel");
        this.btnCtrlPanel.setBounds(40, 300, 200, 30);
        this.btnCtrlPanel.addActionListener(e -> ControlPanel.createInstance());
        frame.add(this.btnCtrlPanel);
    }

    public static void main(String[] args) {
	    SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new Main();

                Main.logic = new RotatorLogic();
                Thread t = new Thread(Main.logic);
                t.start();
            }
        });
    }
}
