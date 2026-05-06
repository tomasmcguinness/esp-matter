set(PRIV_REQUIRES_LIST esp_rcp_update)

idf_component_register(SRC_DIRS          "."
                       INCLUDE_DIRS      "."
                       PRIV_REQUIRES       ${PRIV_REQUIRES_LIST}
                       LDFRAGMENTS "linker.lf")

target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")
