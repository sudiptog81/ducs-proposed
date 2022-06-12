package pro.ghosh.javanotepad;

import javax.xml.crypto.Data;
import java.awt.*;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class EditLogic implements ActionListener {

    @Override
    public void actionPerformed(ActionEvent e) {
        String text = Main.textArea.getText();
        String selectedText = Main.textArea.getSelectedText();

        Clipboard cb = Toolkit.getDefaultToolkit().getSystemClipboard();
        StringSelection data = new StringSelection(selectedText);

        int selStart = Main.textArea.getSelectionStart();
        int selEnd = Main.textArea.getSelectionEnd();

        switch (e.getActionCommand()) {
            case "Cut":
                Main.textArea.setText(text.substring(0, selStart) + text.substring(selEnd));
                Main.textArea.setCaretPosition(selEnd - selectedText.length());
            case "Copy":
                cb.setContents(data, data);
                break;
            case "Paste":
                if (selectedText.length() > 0) {
                    try {
                        Transferable t = cb.getContents(null);
                        String clipboardText = (String) t.getTransferData(DataFlavor.stringFlavor);
                        if (t.isDataFlavorSupported(DataFlavor.stringFlavor)) {
                            Main.textArea.setText(
                                    text.substring(0, selStart) + clipboardText + text.substring(selEnd)
                            );
                            Main.textArea.setCaretPosition(
                                    selEnd + clipboardText.length() - (selEnd - selStart)
                            );
                        }
                    } catch (Exception ignored) {
                    }
                }
                break;
            default:
                break;
        }
    }
}
