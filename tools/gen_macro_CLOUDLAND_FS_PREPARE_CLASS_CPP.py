# SPDX-License-Identifier: MulanPSL-2.0

# generate macro CLOUDLAND_FS_PREPARE_CLASS_CPP defined in src/fs/FsDriverBase.h
#
# created on 2024.7.25 at Haidian, Beijing


from dataclasses import dataclass
import sys


@dataclass
class MethodCallArg:
    arg_type: str = ""
    arg_name: str = ""


@dataclass
class MethodDefine:
    name: str = ""  # without preceding 'fs'
    arg_list_raw: str = ""
    return_type: str = ""


def fetch_method_defines(file: str) -> list[MethodDefine]:
    res: list[MethodDefine] = []


    with open(file, 'r') as f:
        while True:
            line = f.readline()
            if len(line) == 0:
                break


            line = line.strip()

            if not line.startswith("virtual "):
                continue
            
            if not line.endswith(';'):
                line += ' ' + f.readline().strip()
            
            if " fs" not in line:
                continue

            while not line.endswith(';'):
                next_line = f.readline()
                if len(next_line) == 0:
                    break
                line += next_line.strip()


            line = line.rstrip(';')            

            line = line.removeprefix("virtual ")
            
            method_def = MethodDefine()

            fs_tag_pos = line.find(" fs")
            left_quote_pos = line.find("(")
            if fs_tag_pos > left_quote_pos:
                continue

            method_def.return_type = line[0 : fs_tag_pos]
            method_def.name = line[fs_tag_pos + len(" fs") : left_quote_pos]
            method_def.arg_list_raw = line[left_quote_pos + 1 : -1]

            res.append(method_def)


    return res
    


def indent_print(indent: int, content: str, end: str = '\n'):
    print(' ' * indent, end='')
    print(content, end=end)


def cook_arg_list(raw: str) -> list[MethodCallArg]:
    res: list[MethodCallArg] = []
    for arg in raw.split(','):
        arg = arg.strip()
        pos_of_space = arg.rfind(' ')
        call_arg = MethodCallArg()
        call_arg.arg_type = arg[0 : pos_of_space]
        call_arg.arg_name = arg[pos_of_space + 1 : ]
        res.append(call_arg)
    return res



def make_bridges(methods: list[MethodDefine]):
    for idx, method in enumerate(methods):
        indent_print(
            4, 
            f'static {method.return_type} __ ## FsDriver ## _{method.name}({method.arg_list_raw}) {'{'} \\'
        )

        indent_print(8, f'auto i = FsDriver::getInstance(); \\')
        indent_print(8, f'return i->fs{method.name}( \\')

        arg_list = cook_arg_list(method.arg_list_raw)
        for argidx, arg in enumerate(arg_list):
            indent_print(12, arg.arg_name, '')
            if argidx + 1 != len(arg_list):
                print(', \\')
            else:
                print('\\')
        indent_print(8, f'); \\')
        
        indent_print(4, '} \\')


def make_operation_obj(methods: list[MethodDefine]):
    fuse_methods = [
        'getattr', 'readlink', "mknod", 'mkdir', 'unlink', 'rmdir', 'symlink', 'rename',
        'link', 'chmod', 'chown', 'truncate', 'open', 'read', 'write', 'statfs', 'flush',
        'release', 'fsync', 'setxattr', 'getxattr', 'listxattr', 'removexattr', 'opendir',
        'readdir', 'releasedir', 'fsyncdir', 'init', 'destroy', 'access', 'create', 'lock',
        'utimens', 'bmap', 'ioctl', 'poll', 'write_buf', 'read_buf', 'flock', 'fallocate',
        'copy_file_range', 'lseek'
    ]
    print("static fuse_operations fuseOperations = { \\")

    for idx, fuse_method in enumerate(fuse_methods):
        ptr = 'nullptr'
        indent_print(4, f".{fuse_method} = ", end='')
        for method in methods:
            if fuse_method == method.name.lower():
                ptr = f'__ ## FsDriver ## _{method.name}'
                break
        print(f'{ptr}', end='')
        if idx + 1 != len(fuse_methods):
            print(',', end='')
        
        print(' \\')

    print("}; \\")


def make_func_get_fuse_operations():
    print("fuse_operations* FsDriver::getFuseOperations() { \\")
    indent_print(4, "return &fuseOperations; \\")
    print("}")



def main():
    methods = fetch_method_defines("../src/fs/FsDriverBase.h")
    print('// generated by gen_macro_CLOUDLAND_FS_PREPARE_CLASS_CPP.py')
    print("#define CLOUDLAND_FS_PREPARE_CLASS_CPP() \\")
    indent_print(4, "std::unique_ptr<FsDriver> instanceHolder = nullptr; \\")
    indent_print(4, "CLOUDLAND_FS_IMPL_GET_INSTANCE() \\")
    make_bridges(methods)
    make_operation_obj(methods)
    make_func_get_fuse_operations()
    print('// end of generated code.')


if __name__ == '__main__':
    with open("macro_CLOUDLAND_FS_PREPARE_CLASS_CPP.h", "w") as f:
        out = sys.stdout
        sys.stdout = f
        main()
        sys.stdout = out

