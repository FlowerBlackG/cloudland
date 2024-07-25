// SPDX-License-Identifier: MulanPSL-2.0

/*
 *
 * created on 2024.2.10 at Haidian, Beijing
 */

#pragma once


#define CLOUDLAND_OBJ_UTILS_IMPL_CREATE(Type, OptionsType) \
    Type* Type::create(const OptionsType& options) { \
        auto* p = new (std::nothrow) Type; \
        if (p && p->init(options)) { \
            delete p; \
            p = nullptr; \
        } \
        \
        return p; \
    }


#define CLOUDLAND_OBJ_UTILS_DISABLE_COPY(Type) \
    Type(const Type&) = delete; \
    Type& operator = (const Type&) = delete;
