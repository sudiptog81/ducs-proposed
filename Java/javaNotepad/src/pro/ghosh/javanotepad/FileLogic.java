package pro.ghosh.javanotepad;

import javax.swing.*;
import javax.swing.filechooser.FileSystemView;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;

public class FileLogic implements ActionListener {
    static boolean handleDirty() {
        int save = JOptionPane.showConfirmDialog(Main.frame, "Would you like to save the file?", "Save",
                JOptionPane.YES_NO_OPTION);
        if (save == JOptionPane.YES_OPTION) {
            if (Main.filePath != null) {
                save();
            } else {
                saveAs();
            }
            if (Main.dirty) {
                return false;
            }
        }
        return true;
    }

    static void open() {
        if (Main.dirty) {
            if (!handleDirty()) return;
        }

        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
            String path = fileChooser.getSelectedFile().getAbsolutePath();
            try (BufferedReader br = new BufferedReader(new FileReader(path))) {
                StringBuilder sb = new StringBuilder();
                String line = br.readLine();
                while (line != null) {
                    sb.append(line);
                    sb.append(System.lineSeparator());
                    line = br.readLine();
                }
                Main.textArea.setText(sb.toString());
                Main.filePath = path;
                Main.frame.setTitle(fileChooser.getSelectedFile().getName() + " - Notepad");
                Main.dirty = false;
            } catch (Exception ignored) {

            }
        }
    }

    static void save() {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(Main.filePath))) {
            bw.write(Main.textArea.getText());
            Main.dirty = false;
        } catch (Exception ignored) {

        }
    }

    static void saveAs() {
        JFileChooser fileChooser = new JFileChooser(FileSystemView.getFileSystemView().getHomeDirectory());
        fileChooser.setDialogTitle("Save As");
        if (fileChooser.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
            String path = fileChooser.getSelectedFile().getAbsolutePath();
            try (BufferedWriter bw = new BufferedWriter(new FileWriter(path))) {
                bw.write(Main.textArea.getText());
                Main.frame.setTitle(fileChooser.getSelectedFile().getName() + " - Notepad");
                Main.dirty = false;
            } catch (Exception ignored) {

            }
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        switch (e.getActionCommand()) {
            case "Open":
                open();
                return;
            case "Save":
                if (Main.filePath != null) {
                    save();
                    return;
                }
            case "Save As":
                saveAs();
                return;
            default:
                break;
        }
    }
}
