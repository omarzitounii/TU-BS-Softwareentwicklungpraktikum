package com.example.javafx;

import javafx.application.Application;
import javafx.concurrent.Task;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ProgressBar;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.NoSuchElementException;

public class ProgressBar_Benchmark extends Application {

    private ProgressBar ladebalken;

    private int progress = 0;

    @Override
    public void start(Stage primaryStage) {
        ladebalken = new ProgressBar();
        Button updateButton = new Button("update");
        updateButton.setOnAction(e -> update());

        VBox root = new VBox(10);
        root.setPadding(new Insets(10));
        root.getChildren().addAll(ladebalken, updateButton);

        primaryStage.setScene(new Scene(root, 200, 100));
        primaryStage.setTitle("Ladebalken App");
        primaryStage.show();
    }

    private void update() {
        Task<Void> task = new Task<>() {
            @Override
            protected Void call() {
                try (BufferedReader reader = new BufferedReader(new java.io.FileReader("progressbar.txt"))) {

                    String Value = reader.readLine();
                    System.out.println(Value);

                    if (Value != null) {
                        updateProgress(Integer.parseInt(Value), 100);
                    }

                    Thread.sleep(200);

                } catch (FileNotFoundException | NoSuchElementException | NullPointerException e) {
                    e.printStackTrace();
                } catch (IOException | InterruptedException e) {
                    throw new RuntimeException(e);
                }
                return null;
            }
        };

        ladebalken.progressProperty().bind(task.progressProperty());

        task.setOnSucceeded(e -> {
            if (progress == 100) {
                Stage stage = (Stage) ladebalken.getScene().getWindow();
                stage.close();
            }
        });

        Thread thread = new Thread(task);
        thread.setDaemon(true);
        thread.start();
    }

    public void setProgress(int value) {
        progress = value;
    }

    public static void start(String[] args) {
        launch(args);
    }
}

