#include "Commands/Status.hpp"

Commands::Status::Status(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Status::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "Give information on the bot!", cp_bot.me.id);
        commands.chatCommand->set_dm_permission(true);

        ICommand::Init(_commandIds);
    }
}

void Commands::Status::Execute(const dpp::slashcommand_t& event)
{
    ICommand::Execute(event);
    if (event.command.get_command_name() == name)
    {
        //--CPU----------------------------------------
        
        CPUInfo cpuInfo = GetCPUInfo();

        //--RAM----------------------------------------
        
        RAMInfo ramInfo = GetRAMInfo();
        float ramTotal;
        std::string ramTotalUnit;

        float ramAvailable;
        std::string ramAvailableUnit;

        float ramUsed;
        std::string ramUsedUnit;

        ReductByteValue(ramInfo.totalRam, ramTotal, ramTotalUnit);
        ReductByteValue(ramInfo.availableRam, ramAvailable, ramAvailableUnit);
        ReductByteValue(ramInfo.totalRam - ramInfo.availableRam, ramUsed, ramUsedUnit);

        //--DISK---------------------------------------
        
        DiskInfo diskInfo = GetDiskInfo();

        float diskTotal;
        std::string diskTotalUnit;

        float diskAvailable;
        std::string diskAvailableUnit;

        float diskUsed;
        std::string diskUsedUnit;

        ReductByteValue(diskInfo.totalDisk, diskTotal, diskTotalUnit);
        ReductByteValue(diskInfo.availableDisk, diskAvailable, diskAvailableUnit);
        ReductByteValue(diskInfo.totalDisk - diskInfo.availableDisk, diskUsed, diskUsedUnit);
        
        //--GPU----------------------------------------
        
        std::vector<GPUInfo> gpus = GetGPUsInfo();

        dpp::message message;
        message.flags |= dpp::m_using_components_v2;
        message.add_component_v2(
            dpp::component()
            .set_type(dpp::cot_container)
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("# :green_circle: Status: Online")
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("## Running Time: `" + cp_bot.uptime().to_string() + "`")
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_separator)
                .set_divider(true)
                .set_spacing(dpp::sep_large)
            ).add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("## CPU:\n>>> ### - name                       : `" + cpuInfo.name
                    + "`\n### - Sockets                  : `" + std::to_string(cpuInfo.sockets)
                    + "`\n### - Physical Cores    : `" + std::to_string(cpuInfo.physicalCores)
                    + "`\n### - Logical Cores      : `" + std::to_string(cpuInfo.logicalCores)
                    + "`\n### - Base Frequency : `" + (cpuInfo.baseFrequency >= 1000 ? std::format("{} GHz`", (cpuInfo.baseFrequency / 1000.f)) : std::format("{} MHz`",  cpuInfo.baseFrequency))
                )
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content(std::format("## RAM:\n>>> ### - Total                      : `{} {}`\n### - Available              : `{} {}`\n### - Used                      : `{} {}`", ramTotal, ramTotalUnit, ramAvailable, ramAvailableUnit, ramUsed, ramUsedUnit))
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content(std::format("## Disk:\n>>> ### - Total                      : `{} {}`\n### - Available              : `{} {}`\n### - Used                      : `{} {}`", diskTotal, diskTotalUnit, diskAvailable, diskAvailableUnit, diskUsed, diskUsedUnit))
            )
        );

        dpp::component gpuInfo;
        gpuInfo.set_type(dpp::cot_text_display)
        .set_content(gpus.size() == 0 ? "## GPU:\n> ### `NO GPU DETECTED`" : gpus.size() > 1 ? "## GPUs:\n" : "## GPU:\n");
        
        for (GPUInfo gpu : gpus)
        {
            float dedicatedVRAM;
            std::string dedicatedVRAMUnit;
            ReductByteValue(gpu.dedicatedVRAM, dedicatedVRAM, dedicatedVRAMUnit);

            float sharedVRAM;
            std::string sharedVRAMUnit;
            ReductByteValue(gpu.sharedVRAM, sharedVRAM, sharedVRAMUnit);

            gpuInfo.content += std::format("> ### - Name                        : `{}`\n> ### - Dedicated VRAM : `{} {}`\n> ### - Shared VRAM        : `{} {}`\n\n", gpu.name, dedicatedVRAM, dedicatedVRAMUnit, sharedVRAM, sharedVRAMUnit);
        }

        message.components[0].add_component_v2(gpuInfo);

        event.thinking(true, [&, event, message](const dpp::confirmation_callback_t& callback) {
            cp_bot.global_commands_get([&, event, message](const dpp::confirmation_callback_t& callback) mutable
                {
                    if (!callback.is_error())
                    {
                        auto commands = std::get<dpp::slashcommand_map>(callback.value);
                        message.components[0].components[2].content = "## Global Commands: `" + std::to_string(commands.size()) + "`";
                        
                        event.edit_original_response(message);
                    }
                });
            });
    }
}

#ifdef _WIN32
#include <windows.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")
#include <wrl/client.h>
#include <intrin.h>
CPUInfo Commands::Status::GetCPUInfo()
{
    int CPUInfo[4] = { -1 };
    unsigned   nExIds, i = 0;
    char CPUBrandString[0x40] = { '\0' };
    // Get the information associated with each extended ID.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string
        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }

    std::string cpuName = CPUBrandString;
    for (int i = cpuName.size() - 1; i > 0; i--)
    {
        if (cpuName[i] != ' ')
        {
            cpuName.resize(i+1);
            break;
        }
    }

    DWORD len = 0;
    GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &len);

    std::vector<char> buffer(len);
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data());

    if (!GetLogicalProcessorInformationEx(RelationProcessorCore, info, &len))
        return {0};

    uint64_t coreCount = 0;
    uint64_t logicalCount = 0;

    char* ptr = buffer.data();
    char* end = buffer.data() + len;

    while (ptr < end)
    {
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX p = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr);
        if (p->Relationship == RelationProcessorCore)
        {
            coreCount++;
            // Count logical processors for this core
            DWORD l = 0;
            for (DWORD i = 0; i < sizeof(ULONG_PTR) * 8; ++i)
            {
                if (p->Processor.GroupMask[0].Mask & (1ULL << i))
                    logicalCount++;
            }
        }
        ptr += p->Size;
    }

    uint64_t sc = 0;

    // Get number of physical sockets
    len = 0;
    GetLogicalProcessorInformationEx(RelationProcessorPackage, nullptr, &len);
    std::vector<char> buffer2(len);
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pkgInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer2.data());
    if (GetLogicalProcessorInformationEx(RelationProcessorPackage, pkgInfo, &len))
    {
        int socketCount = 0;
        char* ptr2 = buffer2.data();
        char* end2 = buffer2.data() + len;
        while (ptr2 < end2)
        {
            PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX p = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr2);
            if (p->Relationship == RelationProcessorPackage)
                socketCount++;
            ptr2 += p->Size;
        }
        sc = socketCount;
    }

    float freq = 0;

    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {

        DWORD mhz = 0;
        DWORD size = sizeof(DWORD);
        RegQueryValueExA(hKey, "~MHz", nullptr, nullptr, reinterpret_cast<BYTE*>(&mhz), &size);
        RegCloseKey(hKey);

        freq = static_cast<float>(mhz);
    }

    return
    {
        cpuName,
        sc,coreCount, logicalCount,freq
    };
}

RAMInfo Commands::Status::GetRAMInfo()
{
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);
    
    return { mem.ullTotalPhys, mem.ullAvailPhys };
}

DiskInfo Commands::Status::GetDiskInfo()
{
    ULARGE_INTEGER freeBytesAvailable, totalBytes, freeBytes;

    if (GetDiskFreeSpaceExA("C:\\",
        &freeBytesAvailable,
        &totalBytes,
        &freeBytes))
    {
        return { totalBytes.QuadPart, freeBytes.QuadPart };
    }
    else
    {
        return { 0 };
    }
}

std::string WideToUTF8(const wchar_t* wstr)
{
    if (!wstr) return {};
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    std::string result(size - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, result.data(), size, nullptr, nullptr);
    return result;
}


std::vector<GPUInfo> Commands::Status::GetGPUsInfo()
{
    std::vector<GPUInfo> gpus;

    Microsoft::WRL::ComPtr<IDXGIFactory6> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        return gpus;

    for (UINT i = 0;; ++i)
    {
        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
        if (factory->EnumAdapters1(i, &adapter) == DXGI_ERROR_NOT_FOUND)
            break;

        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        // Skip software adapter (WARP)
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            continue;

        GPUInfo gpu;
        gpu.name = WideToUTF8(desc.Description);
        gpu.dedicatedVRAM = desc.DedicatedVideoMemory;
        gpu.sharedVRAM = desc.SharedSystemMemory;

        gpus.push_back(gpu);
    }

    return gpus;

}

#else
#include <sys/statvfs.h>
CPUInfo Commands::Status::GetCPUInfo()
{
    CPUInfo info;
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::set<int> physicalIds;
    std::set<std::pair<int, int>> coreIds; // (physical_id, core_id)
    uint64_t logicalCount = 0;
    float baseFreq = 0.0f;

    while (std::getline(cpuinfo, line))
    {
        if (line.find("model name") == 0 && info.name.empty())
        {
            auto pos = line.find(":");
            if (pos != std::string::npos)
                info.name = line.substr(pos + 2); // skip ": "
        }

        if (line.find("physical id") == 0)
        {
            int pid = std::stoi(line.substr(line.find(":") + 1));
            physicalIds.insert(pid);
        }

        if (line.find("core id") == 0)
        {
            int core_id = std::stoi(line.substr(line.find(":") + 1));
            int physical_id = -1;

            // look back for the last physical_id
            std::ifstream cpuinfo2("/proc/cpuinfo");
            std::string l2;
            int lastPhysical = -1;
            while (std::getline(cpuinfo2, l2))
            {
                if (l2.find("physical id") == 0)
                    lastPhysical = std::stoi(l2.substr(l2.find(":") + 1));
                if (l2.find("core id") == 0)
                {
                    int c = std::stoi(l2.substr(l2.find(":") + 1));
                    if (c == core_id) { physical_id = lastPhysical; break; }
                }
            }

            if (physical_id != -1)
                coreIds.insert({ physical_id, core_id });
        }

        if (line.find("cpu MHz") == 0)
        {
            float freq = std::stof(line.substr(line.find(":") + 1));
            if (baseFreq == 0.0f) // take first entry
                baseFreq = freq;
        }

        if (line.empty())
            logicalCount++;
    }

    info.sockets = physicalIds.empty() ? 1 : physicalIds.size();
    info.physicalCores = coreIds.empty() ? logicalCount : coreIds.size();
    info.logicalCores = logicalCount;
    info.baseFrequency = baseFreq;

    return info;
}

RAMInfo Commands::Status::GetRAMInfo()
{
    RAMInfo info;
    std::ifstream meminfo("/proc/meminfo");
    std::string line;

    while (std::getline(meminfo, line))
    {
        std::istringstream iss(line);
        std::string key;
        uint64_t value;
        std::string unit;

        iss >> key >> value >> unit;

        if (key == "MemTotal:")
            info.totalRam = value * 1024; // kB to B
        else if (key == "MemAvailable:")
            info.availableRam = value * 1024; // kB to B

        if (info.totalRam && info.availableRam)
            break; // found both, no need to continue
    }

    return info;
}

DiskInfo Commands::Status::GetDiskInfo()
{
    DiskInfo info;
    struct statvfs stat;

    if (statvfs(path.c_str(), &stat) != 0)
    {
        // error reading filesystem
        return info;
    }

    info.totalDisk = stat.f_blocks * stat.f_frsize;
    info.availableDisk = stat.f_bavail * stat.f_frsize;

    return info;
}

std::vector<GPUInfo> Commands::Status::GetGPUsInfo()
{
    std::vector<GPUInfo> gpus;

    const std::filesystem::path drmPath = "/sys/class/drm";

    if (!std::filesystem::exists(drmPath))
        return gpus;


    for (const auto& entry : std::filesystem::directory_iterator(drmPath))
    {
        if (!entry.is_directory())
            continue;

        const std::string name = entry.path().filename().string();
        if (name.find("card") != 0) // only card*
            continue;

        std::filesystem::path devicePath = entry.path() / "device";
        if (!std::filesystem::exists(devicePath))
            continue;

        GPUInfo gpu;

        // Read vendor/device
        std::ifstream vendorFile(devicePath / "vendor");
        std::ifstream deviceFile(devicePath / "device");

        if (vendorFile && deviceFile)
        {
            std::string vendorStr, deviceStr;
            vendorFile >> vendorStr;
            deviceFile >> deviceStr;
            gpu.name = "Vendor: " + vendorStr + " Device: " + deviceStr;
        }

        // Try AMD-style VRAM info
        std::ifstream vramFile(devicePath / "mem_info_vram_total");
        if (vramFile)
        {
            uint64_t vram = 0;
            vramFile >> vram;
            gpu.dedicatedVRAM = vram; // already bytes
        }
        else
        {
            // fallback: read first BAR in resource (approximate)
            std::ifstream barFile(devicePath / "resource");
            if (barFile)
            {
                std::string line;
                if (std::getline(barFile, line))
                {
                    std::istringstream iss(line);
                    uint64_t start, end;
                    iss >> std::hex >> start >> end;
                    gpu.dedicatedVRAM = end - start + 1;
                }
            }
        }

        // Estimate shared VRAM as available system RAM
        std::ifstream meminfo("/proc/meminfo");
        std::string memLine;
        while (std::getline(meminfo, memLine))
        {
            if (memLine.find("MemAvailable:") == 0)
            {
                std::istringstream iss(memLine.substr(memLine.find(":") + 1));
                uint64_t availableKB;
                iss >> availableKB;
                gpu.sharedVRAM = availableKB * 1024;
                break;
            }
        }

        gpus.push_back(gpu);
    }

    return gpus;
}

#endif


void Commands::Status::ReductByteValue(uint64_t _value, float& _valueAsFloat, std::string& _unit)
{
    if (_value >= 1073741824)
    {
        _valueAsFloat = _value / 1073741824.f;
        _unit = "GB";
    }
    else if (_value >= 1048576)
    {
        _valueAsFloat = _value / 1048576.f;
        _unit = "MB";
    }
    else if (_value >= 1024)
    {
        _valueAsFloat = _value / 1024.f;
        _unit = "KB";
    }
    else
    {
        //if you endup here you are using a potato
        _valueAsFloat = _value;
        _unit = "B";
    }
}