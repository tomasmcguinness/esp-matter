set(PRIV_REQUIRES_LIST esp_rcp_update)

idf_component_register(SRC_DIRS          "."
                       PRIV_INCLUDE_DIRS  "." "${ESP_MATTER_PATH}/examples/common/utils"
                       PRIV_REQUIRES      ${PRIV_REQUIRES_LIST}
                       LDFRAGMENTS "linker.lf")

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")
