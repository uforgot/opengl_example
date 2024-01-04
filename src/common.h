#ifndef __COMMON_H__
#define __COMMON_H__

#include <iostream>

#include <memory>
#include <string>
#include <optional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

std::optional<std::string> LoadTextFile(const std::string& filename);

#endif // __COMMON_H__