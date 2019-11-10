if (NOT __dependencies_INCLUDED)
  set (__dependencies_INCLUDED ON)

  list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
  set (3RDPARTY_DIR "${PROJECT_SOURCE_DIR}/3rdparty" CACHE PATH "3rdparty directory.")

  # Threads
  set (CMAKE_THREAD_PREFER_PTHREAD ON)
  set (THREADS_PREFER_PTHREAD_FLAG ON)
  find_package (Threads REQUIRED)
  
  # OpenMP
  find_package (OpenMP REQUIRED)

  # OpenCV
  find_package (OpenCV 4 REQUIRED
    COMPONENTS
      imgproc
      highgui
  )

  # libvlc
  find_package (LIBVLC REQUIRED)

  target_include_directories (${PROJECT_NAME}
    PRIVATE
      "${PROJECT_SOURCE_DIR}"
      "${PROJECT_SOURCE_DIR}/src"
      "${PROJECT_BINARY_DIR}"
      "${3RDPARTY_DIR}"
      "${3RDPARTY_DIR}/${CMAKE_SYSTEM_NAME}"
      "${OpenCV_INCLUDE_DIRS}"
      "${LIBVLC_INCLUDE_DIR}"
  )

  target_link_libraries (${PROJECT_NAME}
    PRIVATE
      Threads::Threads
      OpenMP::OpenMP_CXX
      ${OpenCV_LIBS}
      $<$<PLATFORM_ID:Linux>:stdc++fs>
      ${LIBVLC_LIBRARY}
      ${LIBVLCCORE_LIBRARY}
  )

endif ()
