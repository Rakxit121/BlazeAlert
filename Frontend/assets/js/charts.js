import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import org.jfree.chart.*;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.statistics.HistogramDataset;
import org.jfree.data.xy.DefaultXYDataset;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import org.jfree.ui.ApplicationFrame;
import org.jfree.ui.RefineryUtilities;

public class ThingSpeakDataVisualization extends ApplicationFrame {
    private XYSeries series;
    private HistogramDataset histogramDataset;
    private int dataIndex = 0;

    public ThingSpeakDataVisualization(String title) {
        super(title);

        series = new XYSeries("Data");
        XYSeriesCollection dataset = new XYSeriesCollection(series);
        JFreeChart lineChart = ChartFactory.createXYLineChart(
                "ThingSpeak Data Visualization",
                "Time",
                "Value",
                dataset
        );

        XYPlot plot = lineChart.getXYPlot();
        XYLineAndShapeRenderer renderer = new XYLineAndShapeRenderer();
        plot.setRenderer(renderer);

        NumberAxis domainAxis = (NumberAxis) plot.getDomainAxis();
        domainAxis.setAutoRange(true);
        domainAxis.setStandardTickUnits(NumberAxis.createIntegerTickUnits());

        NumberAxis rangeAxis = (NumberAxis) plot.getRangeAxis();
        rangeAxis.setAutoRange(true);

        ChartPanel chartPanel = new ChartPanel(lineChart);
        chartPanel.setPreferredSize(new Dimension(500, 300));
        setContentPane(chartPanel);

        histogramDataset = new HistogramDataset();
        histogramDataset.setType(HistogramType.FREQUENCY);

        JButton updateButton = new JButton("Update");
        updateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateData();
                updateCharts();
            }
        });

        JPanel buttonPanel = new JPanel();
        buttonPanel.add(updateButton);
        add(buttonPanel, BorderLayout.SOUTH);

        pack();
        RefineryUtilities.centerFrameOnScreen(this);
        setVisible(true);
    }

    private void updateData() {
        // Simulate retrieving data from ThingSpeak
        double data = Math.random() * 100; // Replace with actual data retrieval

        series.add(dataIndex++, data);
        histogramDataset.addObservations(new double[]{data});
    }

    private void updateCharts() {
        if (series.getItemCount() > 100) {
            series.remove(0);
        }

        if (histogramDataset.getSeriesCount() > 1) {
            histogramDataset.removeSeries(0);
        }

        JFreeChart lineChart = getChartPanel().getChart();
        XYPlot plot = lineChart.getXYPlot();
        plot.getRenderer().setSeriesShapesVisible(0, true);

        JFreeChart histogramChart = ChartFactory.createHistogram(
                "Data Distribution",
                "Value",
                "Frequency",
                histogramDataset
        );
        ChartPanel histogramPanel = new ChartPanel(histogramChart);
        histogramPanel.setPreferredSize(new Dimension(500, 300));

        JFrame histogramFrame = new JFrame("Histogram");
        histogramFrame.setContentPane(histogramPanel);
        histogramFrame.pack();
        histogramFrame.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                ThingSpeakDataVisualization demo = new ThingSpeakDataVisualization("ThingSpeak Data Visualization");
                Timer timer = new Timer(1000, new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        demo.updateData();
                        demo.updateCharts();
                    }
                });
                timer.start();
            }
        });
    }
}
