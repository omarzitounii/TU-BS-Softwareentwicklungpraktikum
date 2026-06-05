package com.example.javafx;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Label;
import javafx.scene.layout.FlowPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class LineChart_csv extends Application {
    private static List<String[]> CSV_Reader(String path) {
        ArrayList<String[]> Lines = new ArrayList<>();

        try {
            BufferedReader br = new BufferedReader(new FileReader(path));
            int numberOfLines = 2;
            for (int i = 0; i < numberOfLines; ++i) {
                String Line = br.readLine();
                String[] Values = Line.split(",");

                Lines.add(Values);
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return Lines;
    }

    @Override
    public void start(Stage stage) {
        List<String[]> CSV1 = CSV_Reader("./lock_free_result.csv");
        List<String[]> CSV2 = CSV_Reader("./lock_based_result.csv");

        // Definiere die Achsen
        NumberAxis yAxis_Operations = new NumberAxis();
        NumberAxis xAxis_Operations = new NumberAxis();
        yAxis_Operations.setLabel("Clock-time");
        xAxis_Operations.setLabel("Count of Operations");

        NumberAxis yAxis_Latenz_lf = new NumberAxis();
        yAxis_Latenz_lf.setLabel("Latenz");
        NumberAxis xAxis_Latenz_lf = new NumberAxis();
        xAxis_Latenz_lf.setLabel("Clock-time");

        NumberAxis yAxis_Latenz_lb = new NumberAxis();
        yAxis_Latenz_lb.setLabel("Latenz");
        NumberAxis xAxis_Latenz_lb = new NumberAxis();
        xAxis_Latenz_lb.setLabel("Clock-time");

        // Erstelle das Liniendiagramm
        LineChart<Number, Number> lineChart_Operations = new LineChart<>(xAxis_Operations, yAxis_Operations);
        //lineChart_Operations.setTitle("Lock-free against Lock-based");

        LineChart<Number, Number> lineChart_Latenz_lf = new LineChart<>(xAxis_Latenz_lf, yAxis_Latenz_lf);
        //lineChart_Operations.setTitle("Latenz");
        LineChart<Number, Number> lineChart_Latenz_lb = new LineChart<>(xAxis_Latenz_lb, yAxis_Latenz_lb);

        lineChart_Operations.setLegendVisible(false);
        lineChart_Latenz_lf.setLegendVisible(false);
        lineChart_Latenz_lb.setLegendVisible(false);

        // Style Diagram
        lineChart_Latenz_lf.setMaxHeight(200);
        lineChart_Latenz_lb.setMaxHeight(200);

        //lineChart_Latenz_lf.setStyle("-fx-stroke: blue");


        // Erstelle die Datenpunkte für die Graphen
        int maxLatenz_lf = 0,
                maxLatenz_lb = 0,
                minLatenz_lf = 0,
                minLatenz_lb = 0;

        // empty series

        XYChart.Series<Number, Number> series1 = new XYChart.Series<>();
        //series1.setName("Lock-free Overall");
        //System.out.println(CSV1.get(0).length);
        for (int i = 0; i < CSV1.get(0).length; ++i) {
            XYChart.Data data = new XYChart.Data<>(i, Float.parseFloat(CSV1.get(0)[i]));
            data.setNode(new Circle(5, Color.SEAGREEN));
            series1.getData().add(data);
        }

        XYChart.Series<Number, Number> series2 = new XYChart.Series<>();
        //series2.setName("Lock-free Latenz");
        for (int i = 0; i < CSV1.get(1).length; ++i) {
            XYChart.Data data = new XYChart.Data<>(i, Float.parseFloat(CSV1.get(1)[i]));
            data.setNode(new Circle(5, Color.SEAGREEN));
            series2.getData().add(data);
            if (Float.parseFloat(CSV1.get(1)[i]) > Float.parseFloat(CSV1.get(1)[maxLatenz_lf])) maxLatenz_lf = i;
            if (Float.parseFloat(CSV1.get(1)[i]) < Float.parseFloat(CSV1.get(1)[minLatenz_lf])) minLatenz_lf = i;
        }

        XYChart.Series<Number, Number> series3 = new XYChart.Series<>();
        //series3.setName("Lock-based Overall");
        for (int i = 0; i < CSV2.get(0).length; ++i) {
            XYChart.Data data = new XYChart.Data<>(i, Float.parseFloat(CSV2.get(0)[i]));
            data.setNode(new Circle(5, Color.KHAKI));
            series3.getData().add(data);
        }

        XYChart.Series<Number, Number> series4 = new XYChart.Series<>();
        //series4.setName("Lock-based Latenz");
        for (int i = 0; i < CSV2.get(1).length; ++i) {
            XYChart.Data data = new XYChart.Data<>(i, Float.parseFloat(CSV2.get(1)[i]));
            data.setNode(new Circle(5, Color.KHAKI));
            series4.getData().add(data);
            if (Float.parseFloat(CSV2.get(1)[i]) > Float.parseFloat(CSV2.get(1)[maxLatenz_lb])) maxLatenz_lb = i;
            if (Float.parseFloat(CSV2.get(1)[i]) < Float.parseFloat(CSV2.get(1)[minLatenz_lb])) minLatenz_lb = i;
        }


        for (XYChart.Data<Number, Number> data : series2.getData()) {
            if (data.getXValue().doubleValue() == maxLatenz_lf) {
                Circle dataPoint = new Circle();
                dataPoint.setFill(Color.RED);  // Farbe des größten Werts
                dataPoint.setRadius(8);        // Größe des größten Werts

                Label label = new Label("max: " + CSV1.get(1)[maxLatenz_lf]);
                if (data.getXValue().intValue() < series2.getData().size() / 8)
                    data.setXValue(series2.getData().size() / 8);
                data.setYValue(0.75);
                data.setNode(label);
                data.getNode().toFront();
            }

            if (data.getXValue().doubleValue() == minLatenz_lf) {
                Circle dataPoint = new Circle();
                dataPoint.setFill(Color.BLUE);  // Farbe des größten Werts
                dataPoint.setRadius(8);        // Größe des größten Werts

                Label label = new Label("min: " + CSV1.get(1)[minLatenz_lf]);
                if (data.getXValue().intValue() < series2.getData().size() / 8)
                    data.setXValue(series2.getData().size() / 8);
                data.setYValue(0.5);
                data.setNode(label);
                data.getNode().toFront();
            }
        }

        for (XYChart.Data<Number, Number> data : series4.getData()) {
            if (data.getXValue().doubleValue() == maxLatenz_lb) {
                Circle dataPoint = new Circle();
                dataPoint.setFill(Color.RED);  // Farbe des größten Werts
                dataPoint.setRadius(8);        // Größe des größten Werts

                Label label = new Label("max: " + CSV2.get(1)[maxLatenz_lb]);
                if (data.getXValue().intValue() < series4.getData().size() / 8)
                    data.setXValue(series4.getData().size() / 8);
                data.setYValue(0.75);
                data.setNode(label);
                data.getNode().toFront();
            }

            if (data.getXValue().doubleValue() == minLatenz_lb) {
                Circle dataPoint = new Circle();
                dataPoint.setFill(Color.BLUE);  // Farbe des größten Werts
                dataPoint.setRadius(8);        // Größe des größten Werts

                Label label = new Label("min: " + CSV2.get(1)[minLatenz_lb]);
                if (data.getXValue().intValue() < series4.getData().size() / 8)
                    data.setXValue(series4.getData().size() / 8);
                data.setYValue(0.5);
                data.setNode(label);
                data.getNode().toFront();
            }
        }


        // Füge die Graphen dem Liniendiagramm hinzu
        lineChart_Operations.getData().addAll(series1, series3);
        lineChart_Latenz_lf.getData().addAll(series2);
        lineChart_Latenz_lb.getData().addAll(series4);


        // Erstelle Graphennamen
        Label label_op = new Label();
        //lf_label.setFont(new Font(23));
        label_op.setText("Overall: Lock-free against Lock-base");
        label_op.setStyle("-fx-padding: 20px; -fx-font-size: 15px");

        Label label_lf = new Label();
        //lf_label.setFont(new Font(23));
        label_lf.setText("Latenz: Lock-free");
        label_lf.setStyle("-fx-padding: 20px; -fx-font-size: 15px");

        Label label_lb = new Label();
        //lf_label.setFont(new Font(23));
        label_lb.setText("Latenz: Lock-base");
        label_lb.setStyle("-fx-padding: 20px; -fx-font-size: 15px");


        // Style
        //lineChart_Operations.setStyle("-fx-stroke: blue;");
        //lineChart_Latenz_lf.setStyle("-fx-padding: 30px");
        //lineChart_Latenz_lb.setStyle("-fx-padding: 30px");


        // Speichere max/min Latenz


        Label lb_label = new Label();
        //lb_label.setFont(new Font(23));
        lb_label.setText("lock-base Latenz min: " + CSV2.get(1)[minLatenz_lb] + ", max: " + CSV2.get(1)[maxLatenz_lb]);
        lb_label.setStyle("-fx-padding: 20px");


        // erstelle Node von Diagrammen und Labels
        //Group group = new Group();
        //group.getChildren().addAll(lineChart_Operations, lineChart_Latenz_lf, lf_label, lineChart_Latenz_lb, lb_label);

        FlowPane root = new FlowPane();
        FlowPane root_op = new FlowPane();
        FlowPane root_lf = new FlowPane();
        FlowPane root_lb = new FlowPane();

        root_op.getChildren().addAll(label_op, lineChart_Operations);
        root_lf.getChildren().addAll(label_lf, lineChart_Latenz_lf);
        root_lb.getChildren().addAll(label_lb, lineChart_Latenz_lb);
        root.getChildren().addAll(root_op, root_lf, root_lb);

        root_op.setStyle("-fx-border-color: beige");
        root_lf.setStyle("-fx-border-color: beige");
        root_lb.setStyle("-fx-border-color: beige");

        // Erstelle die Szene und zeige sie an
        Scene scene = new Scene(root, 600, 1000);

        stage.setTitle("Lock-free against Lock-based");
        stage.setScene(scene);
        stage.show();

        series4.nodeProperty().get().setStyle("-fx-stroke: blue");
    }

    public static void main(String[] args) {
        launch(args);
    }
}