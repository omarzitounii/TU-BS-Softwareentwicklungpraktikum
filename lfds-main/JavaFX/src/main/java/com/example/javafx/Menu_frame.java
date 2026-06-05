package com.example.javafx;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.Objects;

public class Menu_frame {
    private GroupLayout layout;

    public Menu_frame(String[] args) {
        //create Frame
        initialise_frame();

        //Create Labels
        JLabel threadnr_label = new JLabel("Number of threads:");
        JLabel data_structure_label = new JLabel("Data structure:");
        JLabel inserts_label = new JLabel("Number of inserts:");
        JLabel deletions_label = new JLabel("Number of deletions:");
        JLabel operating_system_label = new JLabel("Betriebssystem:");

        //Create Textfields
        JTextField inserts = new JTextField();
        inserts.setText("100");
        JTextField deletions = new JTextField();
        deletions.setText("100");

        //Create Start Button
        JButton start_button = new JButton("Calculate Performance");

        //Create content for combobox
        String[] data_structures = {
                 "Treibers-Stack", "Priority Queue", "Harris-List / Harris-Set"
        };
        Integer[] thread_number = {
                1, 2, 3, 4, 5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20
        };
        String[] operating_system = {
                "Windows", "Linux / MacOS"
        };

        //Create combobox
        JComboBox<String> data_structure_selection = new JComboBox<>(data_structures);
        JComboBox<Integer> thread_selection = new JComboBox<>(thread_number);
        JComboBox<String> operating_system_selection = new JComboBox<>(operating_system);


        start_button.addActionListener(new ActionListener() {
            class LocalException extends Throwable {
                public LocalException(String thisIsMyException) {
                    super(thisIsMyException);
                }
            }

            //function call for pushing the button
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                try {
                    String selectedDatastructures = Objects.requireNonNull(data_structure_selection.getSelectedItem()).toString();
                    String selectedOperating_System = Objects.requireNonNull(operating_system_selection.getSelectedItem()).toString();
                    String selectedThreads = Objects.requireNonNull(thread_selection.getSelectedItem()).toString();
                    String selectedInserts = inserts.getText();
                    String selectedDeletions = deletions.getText();

                    if (selectedDatastructures.length() == 0
                            || selectedOperating_System.length() == 0
                            || selectedThreads.length() == 0
                            || selectedInserts.length() == 0
                            || selectedDeletions.length() == 0)
                        throw new LocalException("Es dürfen keine Werte leer sein.");

                    Integer.parseInt(selectedInserts);
                    Integer.parseInt(selectedDeletions);

                    if (Integer.parseInt(selectedInserts) < 0 || Integer.parseInt(selectedDeletions) < 0)
                        throw new LocalException("Werte dürfen nicht unter 0 sein!");
                    if (Integer.parseInt(selectedInserts) == 0 && Integer.parseInt(selectedDeletions) == 0)
                        throw new LocalException("Ein Wert muss größer 0 sein!");

                    String selectedDsInt;
                    switch (selectedDatastructures) {
                        case "Harris-List / Harris-Set" -> selectedDsInt = "3";
                        case "Treibers-Stack" -> selectedDsInt = "2";
                        case "Priority Queue" -> selectedDsInt = "1";
                        default -> throw new LocalException("Fehler bei der Auswahl der Datenstruktur.");
                    }

                    BufferedWriter writer = new BufferedWriter(new FileWriter("progressbar.txt"));
                    writer.write("0");
                    writer.close();
                    switch (selectedOperating_System) {
                        case "Linux / MacOS" -> {
                            String cmd = "./benchmark "+ selectedDsInt+" "+ selectedThreads+ " "+selectedInserts+" "+ selectedDeletions;
                            Process process = Runtime.getRuntime().exec(cmd);

                            process.waitFor();
                            if(process.exitValue() != 0 && process.exitValue() !=1){
                                System.exit(-1);
                            }

                            LineChart_csv.main(args);
                            //Applications.main(args);

                        }
                        case "Windows" -> {
                            String cmd = "./benchmark.exe "+ selectedDsInt+" "+ selectedThreads+ " "+selectedInserts+" "+ selectedDeletions;
                            Process process = Runtime.getRuntime().exec(cmd);

                            process.waitFor();

                            LineChart_csv.main(args);
                            //Applications.main(args);
                        }
                        default -> throw new LocalException("Fehler bei Auswahl des Betriebssystems");
                    }
                    File file = new File("./lock_based_result.csv");
                    if (file.delete()) {
                        System.out.println("Datei wurde erfolgreich gelöscht");
                    } else {
                        System.out.println("Fehler beim Löschen der Datei");
                    }

                    File file1 = new File("./lock_free_result.csv");
                    if (file1.delete()) {
                        System.out.println("Datei wurde erfolgreich gelöscht");
                    } else {
                        System.out.println("Fehler beim Löschen der Datei");
                    }
                    System.exit(0);
                } catch (LocalException | Exception e) {
                    e.printStackTrace();
                }
            }
        });

        //create layout
        layout.setHorizontalGroup(
                layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(threadnr_label)
                                .addComponent(inserts_label)
                                .addComponent(deletions_label))
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(thread_selection)
                                .addComponent(inserts)
                                .addComponent(deletions))
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(data_structure_label)
                                .addComponent(operating_system_label))
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(data_structure_selection)
                                .addComponent(operating_system_selection)
                                .addComponent(start_button))
        );
        layout.setVerticalGroup(
                layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                                .addComponent(threadnr_label)
                                .addComponent(thread_selection)
                                .addComponent(data_structure_label)
                                .addComponent(data_structure_selection))
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                                .addComponent(inserts_label)
                                .addComponent(inserts)
                                .addComponent(operating_system_label)
                                .addComponent(operating_system_selection))
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                                .addComponent(deletions_label)
                                .addComponent(deletions))
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                                .addComponent(start_button))
        );
    }

    //Frame initialiser
    private void initialise_frame() {
        JFrame menu = new JFrame("Visualisation for lock free data structures");
        menu.setSize(600, 200);
        menu.setResizable(false);
        menu.setVisible(true);
        menu.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        JPanel panel = initialise_panel();
        menu.add(panel);
    }

    //Panel initialiser
    private JPanel initialise_panel() {
        JPanel panel = new JPanel();
        panel.setVisible(true);
        layout = new GroupLayout(panel);
        panel.setLayout(layout);
        layout.setAutoCreateGaps(true);
        layout.setAutoCreateContainerGaps(true);
        return panel;
    }

}
