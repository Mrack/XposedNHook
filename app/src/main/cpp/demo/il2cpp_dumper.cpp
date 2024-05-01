//
// Created by Mrack on 2024/4/30.
//

#include "il2cpp_dumper.h"
#include "utils.h"
#include "il2cpp-tabledefs.h"
#include "il2cpp-class.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <fcntl.h>

std::map<std::string, void *> Il2CppApi::il2cpp_functions;
void *Il2CppApi::il2cpp_handle;
size_t Il2CppApi::il2cpp_base;


typedef size_t (*solist_get_somain_t)();

typedef size_t *(*solist_get_head_t)();

typedef void *(*soinfo_get_realpath_t)(void *soinfo);

typedef void *(*soinfo_to_handle_t)(void *soinfo);

static solist_get_somain_t solist_get_somain = nullptr;
static solist_get_head_t solist_get_head = nullptr;
static soinfo_get_realpath_t soinfo_get_realpath = nullptr;
static soinfo_to_handle_t soinfo_to_handle = nullptr;


IL2CPP_API(il2cpp_thread_attach, void *, (void * domain), (domain));

IL2CPP_API(il2cpp_domain_get, void *, (void), ());

IL2CPP_API(il2cpp_domain_get_assemblies, void **, (void * domain, size_t * size),
           (domain, size));

IL2CPP_API(il2cpp_assembly_get_image, void *, (void * assembly), (assembly));

IL2CPP_API(il2cpp_image_get_name, char *, (void * image), (image));

IL2CPP_API(il2cpp_class_get_name, char *, (void * klass), (klass));

IL2CPP_API(il2cpp_image_get_class_count, size_t, (void * image), (image));

IL2CPP_API(il2cpp_image_get_class, void *, (void * image, int i), (image, i));

IL2CPP_API(il2cpp_class_get_type, const Il2CppType *, (void * klass), (klass));

IL2CPP_API(il2cpp_type_get_name, char *, (const Il2CppType * ktype), (ktype));

// class
IL2CPP_API(il2cpp_class_from_type, void *, (const Il2CppType * ktype), (ktype));

IL2CPP_API(il2cpp_class_get_namespace, char *, (void * klass), (klass));

IL2CPP_API(il2cpp_class_get_flags, size_t, (void * klass), (klass));

IL2CPP_API(il2cpp_class_is_valuetype, bool, (void * klass), (klass));

IL2CPP_API(il2cpp_class_is_enum, bool, (void * klass), (klass));

IL2CPP_API(il2cpp_class_get_parent, void *, (void * klass), (klass));

IL2CPP_API(il2cpp_class_get_interfaces, void *, (void * klass, void * *iter), (klass, iter));

IL2CPP_API(il2cpp_class_get_fields, void *, (void * klass, void * *iter), (klass, iter));

// field
IL2CPP_API(il2cpp_field_get_flags, size_t, (void * field), (field));

IL2CPP_API(il2cpp_field_get_type, const Il2CppType *, (void * field), (field));

IL2CPP_API(il2cpp_field_get_name, char *, (void * field), (field));

IL2CPP_API(il2cpp_field_static_get_value, char *, (void * field, void * val), (field, val));

IL2CPP_API(il2cpp_field_get_offset, size_t, (void * field), (field));

// prop
IL2CPP_API(il2cpp_class_get_properties, void *, (void * klass, void * *iter), (klass, iter));

IL2CPP_API(il2cpp_property_get_name, char *, (void * prop), (prop));

IL2CPP_API(il2cpp_property_get_get_method, void *, (void * prop), (prop));

IL2CPP_API(il2cpp_property_get_set_method, void *, (void * prop), (prop));

IL2CPP_API(il2cpp_property_get_parent, void *, (void * prop), (prop));


// method
IL2CPP_API(il2cpp_method_get_param_count, size_t, (void * method), (method));

IL2CPP_API(il2cpp_method_get_param, const Il2CppType *, (void * method, size_t i), (method, i));

IL2CPP_API(il2cpp_class_get_methods, MethodInfo *, (void * klass, void * *iter), (klass, iter));

IL2CPP_API(il2cpp_method_get_name, char *, (void * method), (method));

IL2CPP_API(il2cpp_method_get_flags, size_t, (void * method, uint32_t * iflag), (method, iflag));

IL2CPP_API(il2cpp_method_get_return_type, const Il2CppType *, (void * method), (method));

IL2CPP_API(il2cpp_method_get_declaring_type, void *, (void * method), (method));

IL2CPP_API(il2cpp_method_get_class, void *, (void * method), (method));

IL2CPP_API(il2cpp_method_has_attribute, bool, (void * method, void * attr_class),
           (method, attr_class));

IL2CPP_API(il2cpp_method_get_token, size_t, (void * method), (method));

IL2CPP_API(il2cpp_method_get_param_name, char *, (void * method, size_t i), (method, i));

IL2CPP_API(il2cpp_method_get_object, void *, (void * method, void * refclass),
           (method, refclass));

IL2CPP_API(il2cpp_method_is_generic, bool, (void * method), (method));

IL2CPP_API(il2cpp_method_is_inflated, bool, (void * method), (method));

IL2CPP_API(il2cpp_method_is_instance, bool, (void * method), (method));

IL2CPP_API(il2cpp_method_get_from_reflection, void *, (void * method), (method));

IL2CPP_API(il2cpp_type_is_byref, bool, (const Il2CppType * type), (type));

IL2CPP_API(il2cpp_get_corlib, void *, (void), ());

IL2CPP_API(il2cpp_class_from_name, void *, (void * image, const char *namespaze, const char *name),
           (image, namespaze, name));

IL2CPP_API(il2cpp_class_get_method_from_name, MethodInfo *,
           (void * klass, const char *name, int argsCount),
           (klass, name, argsCount));

IL2CPP_API(il2cpp_string_new, void *, (const char *str), (str));

IL2CPP_API(il2cpp_class_from_system_type, void *, (Il2CppReflectionType * type), (type));

bool _il2cpp_type_is_byref(const Il2CppType *type) {
    auto byref = type->byref;
    if (il2cpp_type_is_byref_ptr) {
        byref = il2cpp_type_is_byref(type);
    }
    return byref;
}


std::string get_method_modifier(uint32_t flags) {
    std::stringstream outPut;
    auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;
    switch (access) {
        case METHOD_ATTRIBUTE_PRIVATE:
            outPut << "private ";
            break;
        case METHOD_ATTRIBUTE_PUBLIC:
            outPut << "public ";
            break;
        case METHOD_ATTRIBUTE_FAMILY:
            outPut << "protected ";
            break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
            outPut << "internal ";
            break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & METHOD_ATTRIBUTE_STATIC) {
        outPut << "static ";
    }
    if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_FINAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "sealed override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
            outPut << "virtual ";
        } else {
            outPut << "override ";
        }
    }
    if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
        outPut << "extern ";
    }
    return outPut.str();
}


std::string dump_method(void *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Methods\n";
    void *iter = nullptr;
    while (auto method = il2cpp_class_get_methods(klass, &iter)) {
        //TODO attribute
        if (method->methodPointer) {
            outPut << "\t// RVA: 0x";
            outPut << std::hex << (uint64_t) method->methodPointer - Il2CppApi::il2cpp_base;
            outPut << " VA: 0x";
            outPut << std::hex << (uint64_t) method->methodPointer;
        } else {
            outPut << "\t// RVA: 0x VA: 0x0";
        }
        /*if (method->slot != 65535) {
            outPut << " Slot: " << std::dec << method->slot;
        }*/
        outPut << "\n\t";
        uint32_t iflags = 0;
        auto flags = il2cpp_method_get_flags(method, &iflags);
        outPut << get_method_modifier(flags);
        //TODO genericContainerIndex
        auto return_type = il2cpp_method_get_return_type(method);
        if (_il2cpp_type_is_byref(return_type)) {
            outPut << "ref ";
        }
        auto return_class = il2cpp_class_from_type(return_type);
        outPut << il2cpp_class_get_name(return_class) << " "
               << il2cpp_method_get_name(method)
               << "(";
        auto param_count = il2cpp_method_get_param_count(method);
        for (int i = 0; i < (int) param_count; ++i) {
            auto param = il2cpp_method_get_param(method, i);
            auto attrs = param->attrs;
            if (_il2cpp_type_is_byref(param)) {
                if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN)) {
                    outPut << "out ";
                } else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT)) {
                    outPut << "in ";
                } else {
                    outPut << "ref ";
                }
            } else {
                if (attrs & PARAM_ATTRIBUTE_IN) {
                    outPut << "[In] ";
                }
                if (attrs & PARAM_ATTRIBUTE_OUT) {
                    outPut << "[Out] ";
                }
            }
            auto parameter_class = il2cpp_class_from_type(param);
            outPut << il2cpp_class_get_name(parameter_class) << " "
                   << il2cpp_method_get_param_name(method, i);
            outPut << ", ";
        }
        if (param_count > 0) {
            outPut.seekp(-2, outPut.cur);
        }
        outPut << ") { }\n";
        //TODO GenericInstMethod
    }
    return outPut.str();
}

std::string dump_property(void *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Properties\n";
    void *iter = nullptr;
    while (auto prop_const = il2cpp_class_get_properties(klass, &iter)) {
        //TODO attribute
        auto prop = prop_const;
        auto get = il2cpp_property_get_get_method(prop);
        auto set = il2cpp_property_get_set_method(prop);
        auto prop_name = il2cpp_property_get_name(prop);
        outPut << "\t";
        void *prop_class = nullptr;
        uint32_t iflags = 0;
        if (get) {
            outPut << get_method_modifier(il2cpp_method_get_flags(get, &iflags));
            prop_class = il2cpp_class_from_type(
                    il2cpp_method_get_return_type(get));
        } else if (set) {
            outPut << get_method_modifier(il2cpp_method_get_flags(set, &iflags));
            auto param = il2cpp_method_get_param(set, 0);
            prop_class = il2cpp_class_from_type(param);
        }
        if (prop_class) {
            outPut << il2cpp_class_get_name(prop_class) << " " << prop_name << " { ";
            if (get) {
                outPut << "get; ";
            }
            if (set) {
                outPut << "set; ";
            }
            outPut << "}\n";
        } else {
            if (prop_name) {
                outPut << " // unknown property " << prop_name;
            }
        }
    }
    return outPut.str();
}

std::string dump_field(void *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Fields\n";
    auto is_enum = il2cpp_class_is_enum(klass);
    void *iter = nullptr;
    while (auto field = il2cpp_class_get_fields(klass, &iter)) {
        //TODO attribute
        outPut << "\t";
        auto attrs = il2cpp_field_get_flags(field);
        auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
        switch (access) {
            case FIELD_ATTRIBUTE_PRIVATE:
                outPut << "private ";
                break;
            case FIELD_ATTRIBUTE_PUBLIC:
                outPut << "public ";
                break;
            case FIELD_ATTRIBUTE_FAMILY:
                outPut << "protected ";
                break;
            case FIELD_ATTRIBUTE_ASSEMBLY:
            case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                outPut << "internal ";
                break;
            case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                outPut << "protected internal ";
                break;
        }
        if (attrs & FIELD_ATTRIBUTE_LITERAL) {
            outPut << "const ";
        } else {
            if (attrs & FIELD_ATTRIBUTE_STATIC) {
                outPut << "static ";
            }
            if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                outPut << "readonly ";
            }
        }
        auto field_type = il2cpp_field_get_type(field);
        auto field_class = il2cpp_class_from_type(field_type);
        outPut << il2cpp_class_get_name(field_class) << " "
               << il2cpp_field_get_name(field);
        //TODO 获取构造函数初始化后的字段值
        if (attrs & FIELD_ATTRIBUTE_LITERAL && is_enum) {
            uint64_t val = 0;
            il2cpp_field_static_get_value(field, &val);
            outPut << " = " << std::dec << val;
        }
        outPut << "; // 0x" << std::hex << il2cpp_field_get_offset(field) << "\n";
    }
    return outPut.str();
}

std::string dump_type(const Il2CppType *type) {
    std::stringstream outPut;
    auto *klass = il2cpp_class_from_type(type);
    outPut << "\n// Namespace: " << il2cpp_class_get_namespace(klass) << "\n";
    auto flags = il2cpp_class_get_flags(klass);
    if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
        outPut << "[Serializable]\n";
    }
    //TODO attribute
    auto is_valuetype = il2cpp_class_is_valuetype(klass);
    auto is_enum = il2cpp_class_is_enum(klass);
    auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
    switch (visibility) {
        case TYPE_ATTRIBUTE_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_PUBLIC:
            outPut << "public ";
            break;
        case TYPE_ATTRIBUTE_NOT_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
        case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
            outPut << "internal ";
            break;
        case TYPE_ATTRIBUTE_NESTED_PRIVATE:
            outPut << "private ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAMILY:
            outPut << "protected ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "static ";
    } else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
    } else if (!is_valuetype && !is_enum && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "sealed ";
    }
    if (flags & TYPE_ATTRIBUTE_INTERFACE) {
        outPut << "interface ";
    } else if (is_enum) {
        outPut << "enum ";
    } else if (is_valuetype) {
        outPut << "struct ";
    } else {
        outPut << "class ";
    }
    outPut << il2cpp_class_get_name(klass); //TODO genericContainerIndex
    std::vector<std::string> extends;
    auto parent = il2cpp_class_get_parent(klass);
    if (!is_valuetype && !is_enum && parent) {
        auto parent_type = il2cpp_class_get_type(parent);
        if (parent_type->type != IL2CPP_TYPE_OBJECT) {
            extends.emplace_back(il2cpp_class_get_name(parent));
        }
    }
    void *iter = nullptr;
    while (auto itf = il2cpp_class_get_interfaces(klass, &iter)) {
        extends.emplace_back(il2cpp_class_get_name(itf));
    }
    if (!extends.empty()) {
        outPut << " : " << extends[0];
        for (int i = 1; i < extends.size(); ++i) {
            outPut << ", " << extends[i];
        }
    }
    outPut << "\n{";
    outPut << dump_field(klass);
    outPut << dump_property(klass);
    outPut << dump_method(klass);
    //TODO EventInfo
    outPut << "}\n";
    return outPut.str();
}
//
//void *open_address;
//install_hook_name(open, int, const char *pathname, int flags) {
//    int res = orig_open(pathname, flags);
//    LOGD("open: %s, %d", pathname, res);
//    if (strstr(pathname, "global-metadata.dat") != nullptr) {
//        DobbyDestroy((void *) open_address);
//    }
//    return res;
//}

void dumpIL2cpp(const char *path) {
//    open_address = DobbySymbolResolver(nullptr, "open");
//    install_hook_open(open_address);
    std::thread([path]() {
        while (true) {
            void *handle = get_il2cpp_handle();
            if (handle == nullptr) {
                sleep(3);
                continue;
            }
            sleep(1);
            int version = 23;

            for (int i = 0; i < 3; ++i) {
                auto meta = find_path_from_maps("global-metadata.dat");
                if (meta) {
                    auto info = find_info_from_maps(
                            "global-metadata.dat");
                    void *addr = (void *) info.first;
                    unsigned int magic = *(unsigned int *) addr;
                    if (magic != 0xFAB11BAF) {
                        LOGD("global-metadata.dat: magic check failed");
                        continue;
                    }
                    version = *(int *) ((char *) addr + 4);
                    LOGD("version: %d, global-metadata.dat: %s", version, meta);
                    break;
                } else {
                    sleep(1);
                    continue;
                }
            }


            if (dump(version, path)) {
                break;
            }
            sleep(1);
        }
    }).detach();

}

bool dump(int version, const char *path) {
    void *domain = il2cpp_domain_get();
    if (domain == nullptr) {
        return false;
    }
    LOGD("Starting dump... ");
    il2cpp_thread_attach(domain);
    size_t size = 0;
    void **assemblies = il2cpp_domain_get_assemblies(domain, &size);
    if (size == 0) {
        return false;
    }

    std::stringstream imageOutput;

    for (size_t i = 0; i < size; i++) {
        void *image = il2cpp_assembly_get_image(assemblies[i]);
        const char *name = il2cpp_image_get_name(image);
        imageOutput << "// Image " << i << ": " << name << "\n";
    }

    std::vector<std::string> outPuts;
    if (il2cpp_image_get_class_ptr) {
        LOGI("Version greater than 2018.3");
        for (int i = 0; i < size; ++i) {
            auto image = il2cpp_assembly_get_image(assemblies[i]);
            std::stringstream imageStr;
            imageStr << "\n// Dll : " << il2cpp_image_get_name(image);
            auto classCount = il2cpp_image_get_class_count(image);
            for (int j = 0; j < classCount; ++j) {
                auto klass = il2cpp_image_get_class(image, j);
                auto type = il2cpp_class_get_type(klass);
                auto outPut = imageStr.str() + dump_type(type);
                outPuts.push_back(outPut);
            }
        }
    } else {
        LOGI("Version less than 2018.3");
        //使用反射
        auto corlib = il2cpp_get_corlib();
        auto assemblyClass = il2cpp_class_from_name(corlib, "System.Reflection", "Assembly");
        auto assemblyLoad = il2cpp_class_get_method_from_name(assemblyClass, "Load", 1);
        auto assemblyGetTypes = il2cpp_class_get_method_from_name(assemblyClass, "GetTypes", 0);
        if (assemblyLoad && assemblyLoad->methodPointer) {
            LOGI("Assembly::Load: %p", assemblyLoad->methodPointer);
        } else {
            LOGI("miss Assembly::Load");
            return false;
        }
        if (assemblyGetTypes && assemblyGetTypes->methodPointer) {
            LOGI("Assembly::GetTypes: %p", assemblyGetTypes->methodPointer);
        } else {
            LOGI("miss Assembly::GetTypes");
            return false;
        }
        typedef void *(*Assembly_Load_ftnb24)(void *, void *);
        typedef void *(*Assembly_Load_ftn)(void *, void *, void *);
        typedef Il2CppArray *(*Assembly_GetTypes_ftn)(void *, void *);
        for (int i = 0; i < size; ++i) {
            auto image = il2cpp_assembly_get_image(assemblies[i]);
            std::stringstream imageStr;
            auto image_name = il2cpp_image_get_name(image);
            imageStr << "\n// Dll : " << image_name;
            //LOGD("image name : %s", image->name);
            auto imageName = std::string(image_name);
            auto pos = imageName.rfind('.');
            auto imageNameNoExt = imageName.substr(0, pos);
            auto assemblyFileName = il2cpp_string_new(imageNameNoExt.data());
            void *reflectionAssembly;
            if (version > 24) {
                reflectionAssembly = ((Assembly_Load_ftnb24) assemblyLoad->methodPointer)(
                        assemblyFileName,
                        nullptr);
            } else {
                reflectionAssembly = ((Assembly_Load_ftn) assemblyLoad->methodPointer)(nullptr,
                                                                                       assemblyFileName,
                                                                                       nullptr);
            }
            auto reflectionTypes = ((Assembly_GetTypes_ftn) assemblyGetTypes->methodPointer)(
                    reflectionAssembly, nullptr);
            auto items = reflectionTypes->vector;
            for (int j = 0; j < reflectionTypes->max_length; ++j) {
                auto klass = il2cpp_class_from_system_type((Il2CppReflectionType *) items[j]);
                auto type = il2cpp_class_get_type(klass);
                //LOGD("type name : %s", il2cpp_type_get_name(type));
                auto outPut = imageStr.str() + dump_type(type);
                outPuts.push_back(outPut);
            }
        }
    }
    auto outPath = std::string(path).append("/").append("dump.cs");
    LOGI("write dump file: %s", outPath.c_str());
    std::ofstream outStream(outPath);
    outStream << imageOutput.str();
    auto count = outPuts.size();
    for (int i = 0; i < count; ++i) {
        outStream << outPuts[i];
    }
    outStream.close();
    LOGI("dump done!");
    return true;
}

void *get_il2cpp_handle() {
    if (solist_get_somain == nullptr || solist_get_head == nullptr ||
        soinfo_get_realpath == nullptr || soinfo_to_handle == nullptr) {
        solist_get_somain = (solist_get_somain_t) (get_address_from_module(
                get_linker_path(), "solist_get_somain"));
        solist_get_head = (solist_get_head_t) (get_address_from_module(
                get_linker_path(), "solist_get_head"));
        soinfo_get_realpath = (soinfo_get_realpath_t) (get_address_from_module(
                get_linker_path(), "__dl__ZNK6soinfo12get_realpathEv"));
        soinfo_to_handle = (soinfo_to_handle_t) (get_address_from_module(
                get_linker_path(), "__dl__ZN6soinfo9to_handleEv"));
    }

    if (soinfo_get_realpath == nullptr || soinfo_to_handle == nullptr) {
        LOGD("get_il2cpp_handle: soinfo_get_realpath or soinfo_to_handle not found");
        return nullptr;
    }
    if (solist_get_head == nullptr || solist_get_somain == nullptr) {
        LOGD("get_il2cpp_handle: solist_get_head or solist_get_somain not found");
        return nullptr;
    }

    auto solist_head = (*solist_get_head)();
    auto somain = (*solist_get_somain)();

    size_t STRUCT_OFFSET_solist_next = 0;
    for (int i = 0; i < 1024 / sizeof(void *); i++) {
        if (solist_head[i] == somain) {
            STRUCT_OFFSET_solist_next = i * sizeof(void *);
            break;
        }
    }
    std::vector<void *> linker_solist;
    void *current = solist_head;
    while (current != nullptr) {
        linker_solist.push_back(current);
        current = *(void **) ((size_t) current + STRUCT_OFFSET_solist_next);
    }

    for (auto soinfo: linker_solist) {
        auto realpath = (*soinfo_get_realpath)(soinfo);
        if (realpath == nullptr) {
            continue;
        }
        auto handle = (*soinfo_to_handle)(soinfo);
        if (handle == nullptr) {
            continue;
        }
        if (strstr((const char *) realpath, "libil2cpp.so") != nullptr) {
            Il2CppApi::il2cpp_handle = handle;
            if (il2cpp_domain_get()) {
                Dl_info info;
                dladdr((void *) il2cpp_domain_get_ptr, &info);
                Il2CppApi::il2cpp_base = (size_t) info.dli_fbase;
                LOGD("il2cpp_base: 0x%zx", Il2CppApi::il2cpp_base);
                return handle;
            }
            return nullptr;
        }
    }
    Il2CppApi::il2cpp_handle = nullptr;
    Il2CppApi::il2cpp_base = 0;
    return nullptr;


}