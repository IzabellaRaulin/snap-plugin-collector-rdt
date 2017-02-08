#include <iostream>
#include <sstream>

#include "google/protobuf/repeated_field.h"
#include "snap/plugin.h"
#include "rdt/rdt.hpp"

std::string print_ns(Plugin::Metric metric) {
    std::ostringstream stream;
    for (auto iter = metric.ns().begin(); iter != metric.ns().end(); iter++) {
        stream << "/" << (*iter).value;
    }
    return stream.str();
}

int main() {
    try {
        rdt::Collector rdt;

        rpc::ConfigMap map;
        Plugin::Config config(map);

        auto metric_types = rdt.get_metric_types(config);

        for (auto iter = metric_types.begin(); iter != metric_types.end(); iter++) {
            std::cout << "namespace: " << print_ns(*iter) << " | int data: " << (*iter).get_int_data() << "| float data: " << (*iter).get_float64_data() << " | dynamic elements: "<< (*iter).dynamic_ns_elements().size() << std::endl;
        }

        std::cout << "---------------------------------------------------------" << std::endl;

        std::vector<Plugin::Metric> collected_metrics;
        rdt.collect_metrics(collected_metrics);

        for (auto iter = collected_metrics.begin(); iter != collected_metrics.end(); iter++) {
            std::cout << "namespace: " << print_ns(*iter) << " | int data: " << (*iter).get_int_data() << "| float data: " << (*iter).get_float64_data() << " | dynamic elements: "<< (*iter).dynamic_ns_elements().size() << std::endl;
        }

        return 0;
    } catch (const char* what) {
        fprintf(stderr, "Cannot launch RDT Collector: %s\n", what);
        return -1;
    }
}
