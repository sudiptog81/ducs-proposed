package pro.ghosh.javanotepad;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.undo.UndoManager;
import java.awt.*;
import java.awt.event.*;

public class Main {
    static boolean dirty = false;
    static JTextPane textArea;
    static String filePath;
    static final JFrame frame = new JFrame("Notepad");

    FileLogic fileLogic;
    EditLogic editLogic;

    JMenuBar menuBar, statusBar;
    JMenu fileMenu, editMenu, statusMenu;
    JMenuItem fm1, fm2, fm3, fm4,
            em1, em2, em3;

    static int countWords(String str) {
        if (str == null || str.isEmpty())
            return 0;
        return str.split("\\s+").length;
    }

    Main() {
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setSize(400, 500);
        frame.setLayout(new BorderLayout());

        this.menuBar = new JMenuBar();

        this.fileLogic = new FileLogic();
        this.fileMenu = new JMenu("File");
        this.fm1 = new JMenuItem("Open");
        this.fm1.addActionListener(fileLogic);
        this.fm2 = new JMenuItem("Save");
        this.fm2.addActionListener(fileLogic);
        this.fm3 = new JMenuItem("Save As");
        this.fm3.addActionListener(fileLogic);
        this.fm4 = new JMenuItem("Exit");
        this.fm4.addActionListener(e -> frame.dispose());
        this.fileMenu.add(this.fm1);
        this.fileMenu.add(this.fm2);
        this.fileMenu.add(this.fm3);
        this.fileMenu.add(this.fm4);
        this.menuBar.add(fileMenu);

        this.editLogic = new EditLogic();
        this.editMenu = new JMenu("Edit");
        this.em1 = new JMenuItem("Cut");
        this.em1.addActionListener(editLogic);
        this.em2 = new JMenuItem("Copy");
        this.em2.addActionListener(editLogic);
        this.em3 = new JMenuItem("Paste");
        this.em3.addActionListener(editLogic);
        this.editMenu.add(this.em1);
        this.editMenu.add(this.em2);
        this.editMenu.add(this.em3);
        this.menuBar.add(editMenu);

        frame.setJMenuBar(menuBar);

        textArea = new JTextPane();
        textArea.setFont(new Font("Consolas", Font.PLAIN, 15));
        textArea.setBorder(BorderFactory.createCompoundBorder(
                textArea.getBorder(),
                BorderFactory.createEmptyBorder(5, 5, 5, 5))
        );

        KeyStroke undoKeyStroke = KeyStroke.getKeyStroke(KeyEvent.VK_Z, InputEvent.CTRL_MASK);
        KeyStroke redoKeyStroke = KeyStroke.getKeyStroke(KeyEvent.VK_Y, InputEvent.CTRL_MASK);
        UndoManager undoManager = new UndoManager();
        textArea.getDocument().addUndoableEditListener(e -> undoManager.addEdit(e.getEdit()));
        textArea.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(undoKeyStroke, "undoKeyStroke");
        textArea.getActionMap().put("undoKeyStroke", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    undoManager.undo();
                } catch (Exception ignored) {

                }
            }
        });
        textArea.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(redoKeyStroke, "redoKeyStroke");
        textArea.getActionMap().put("redoKeyStroke", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    undoManager.redo();
                } catch (Exception ignored) {
                }
            }
        });

        JScrollPane areaScrollPane = new JScrollPane(
                textArea,
                JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                JScrollPane.HORIZONTAL_SCROLLBAR_NEVER
        );
        frame.add(areaScrollPane, BorderLayout.NORTH);

        this.statusBar = new JMenuBar();
        this.statusMenu = new JMenu("0 characters");
        this.statusBar.add(statusMenu);
        frame.add(this.statusBar, BorderLayout.SOUTH);

        frame.setVisible(true);
        frame.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                super.componentResized(e);
                areaScrollPane.setPreferredSize(new Dimension(
                        frame.getWidth(),
                        frame.getHeight() - 85
                ));
            }
        });

        textArea.getDocument().addDocumentListener(new DocumentListener() {
            void handleChange() {
                statusMenu.setText(countWords(textArea.getText()) + " words "
                        + textArea.getText().length() + " characters");
                dirty = true;
                if (!frame.getTitle().contains("[*] "))
                    frame.setTitle("[*] " + frame.getTitle());
            }

            @Override
            public void insertUpdate(DocumentEvent e) {
                handleChange();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                handleChange();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                handleChange();
            }
        });

        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                if (dirty) {
                    if (!FileLogic.handleDirty()) return;
                }

                super.windowClosing(e);
            }
        });
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(Main::new);
    }
}
