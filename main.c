#include <stdio.h>
#include <unistd.h>

#include <nvml.h>

int main() {
    nvmlReturn_t ret;

    ret = nvmlInit();
    if (ret != NVML_SUCCESS) {
        printf("Failed to initialize NVML: %s\n", nvmlErrorString(ret));
        return 1;
    }

    int n;
    int cuda_v;
    char driver[256];

    ret = nvmlDeviceGetCount(&n);
    printf("GPU Num: %d\n", n);

    // Get driver version.
    ret = nvmlSystemGetDriverVersion(driver, 256);
    printf("Driver Version: %s\n", driver);

    // Get cuda driver version.
    ret  = nvmlSystemGetCudaDriverVersion(&cuda_v);
    printf("CUDA Version: %d\n", cuda_v);

    for (int i = 0; i < n; i++) {
        nvmlDevice_t dev;
        nvmlMemory_t memory;
        nvmlPciInfo_t pcie;
        nvmlPstates_t power_state;
        int fan_speed;
        char uuid[256], bus[256];
        unsigned int temperature, power_limit, power_default_limit, power_usage, enforced_power_limit, \
            power_min_limit, power_max_limit, sm_clock, mem_clock;

        printf("\nGPU-%d:\n", i);

        // Get handle by index.
        nvmlDeviceGetHandleByIndex(i, &dev);

        // Get uuid.
        nvmlDeviceGetUUID(dev, uuid, 256);
        printf("  UUID: %s\n", uuid);

        // Get fan speed.
        nvmlDeviceGetFanSpeed(dev, &fan_speed);
        printf("  Fan speed: %d\n", fan_speed);

        // Get memory information.
        nvmlDeviceGetMemoryInfo(dev, &memory);
        printf("  Memory:\n");
        printf("    - Total: %llu MiB\n", memory.total / (1024 * 1024));
        printf("    - Used: %llu MiB\n", memory.used / (1024 * 1024));
        printf("    - Free: %llu MiB\n", memory.free / (1024 * 1024));

        // Get temperature.
        nvmlDeviceGetTemperature(dev, NVML_TEMPERATURE_GPU, &temperature);
        printf("  Temperature: %u C\n", temperature);

        // Get pcie information.
        nvmlDeviceGetPciInfo(dev, &pcie);
        printf("  PCIE:\n");
        printf("    - Bus ID: %s\n", pcie.busId);


        // Get power information.
        nvmlDeviceGetPowerState(dev, &power_state);
        nvmlDeviceGetPowerUsage(dev, &power_usage);
        nvmlDeviceGetPowerManagementLimitConstraints(dev, &power_min_limit, &power_max_limit);
        nvmlDeviceGetPowerManagementLimit(dev, &power_limit);
        nvmlDeviceGetPowerManagementDefaultLimit(dev, &power_default_limit);
        nvmlDeviceGetEnforcedPowerLimit(dev, &enforced_power_limit);
        printf("  Power\n");
        printf("    - State: %d\n", power_state);
        printf("    - Usage: %d W\n", power_usage);
        printf("    - Min limit: %d W\n", power_min_limit);
        printf("    - Max limit: %d W\n", power_max_limit);
        printf("    - Limit: %d W\n", power_limit);
        printf("    - Default limit: %d W\n", power_default_limit);
        printf("    - Enforced limit: %d W\n", enforced_power_limit);

        // Get clock
        nvmlDeviceGetClockInfo(dev, NVML_CLOCK_SM, &sm_clock);
        nvmlDeviceGetClockInfo(dev, NVML_CLOCK_MEM, &mem_clock);
        printf("  Clock\n");
        printf("    - SM: %d MHz\n", sm_clock);
        printf("    - Mem: %d MHz\n", mem_clock);
    }

    nvmlShutdown();
}
