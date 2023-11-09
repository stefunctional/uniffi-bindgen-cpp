/* This file was generated by uniffi-bindgen-cpp. */

{%- let namespace = ci.namespace() %}

{%- import "macros.cpp" as macros %}

#include <string>

#include "{{ ci.namespace() }}.hpp"

namespace {{ namespace }} {
    namespace uniffi {
        template<class> inline constexpr bool always_false_v = false;

        template<typename F>
        void check_rust_call(
            const RustCallStatus &status,
            F error_cb
        ) {
            switch (status.code) {
            case 0:
                return;

            case 1:
                if constexpr (!std::is_null_pointer_v<F>) {
                    error_cb(status.error_buf)->throw_underlying();
                }
                break;

            case 2:
                if (status.error_buf.len > 0) {
                    throw std::runtime_error({{ namespace }}::uniffi::{{ Type::String.borrow()|lift_fn }}(status.error_buf));
                }

                throw std::runtime_error("A Rust panic has occurred");
            }

            throw std::runtime_error("Unexpected Rust call status");
        }

        template<typename F, typename EF, typename... Args, typename R = std::invoke_result_t<F, Args..., RustCallStatus *>>
        R rust_call(F f, EF error_cb, Args... args) {
            RustCallStatus status = { 0 };

            if constexpr (std::is_void_v<R>) {
                f(args..., &status);
                {{ namespace }}::uniffi::check_rust_call(status, error_cb);
            } else {
                auto ret = f(args..., &status);

                {{ namespace }}::uniffi::check_rust_call(status, error_cb);

                return ret;
            }
        }
    }
}

{{ type_helper_code }}

{% include "rust_buf_tmpl.cpp" %}

namespace {{ namespace }} {
    {%- for func in ci.function_definitions() %}
    {% include "fn_def.cpp" -%}
    {% endfor %}
}
