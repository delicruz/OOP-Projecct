#ifndef SKILLSMANAGER_H
#define SKILLSMANAGER_H

#include <memory>
#include <string>
#include "Skill.h"

std::unique_ptr<Skill> createSkillFromData(const std::string& line);

#endif