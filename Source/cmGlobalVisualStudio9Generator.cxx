/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "windows.h" // this must be first to define GetCurrentDirectory
#include "cmGlobalVisualStudio9Generator.h"
#include "cmLocalVisualStudio7Generator.h"
#include "cmMakefile.h"
#include "cmake.h"

static const char vs9Win32generaorName[] = "Visual Studio 9 2008";
static const char vs9Win64generaorName[] = "Visual Studio 8 2005 Win64";
static const char vs9IA64generaorName[] = "Visual Studio 9 2008 IA64";

class cmGlobalVisualStudio9Generator::Factory
  : public cmGlobalGeneratorFactory
{
public:
  virtual cmGlobalGenerator* CreateGlobalGenerator(const char* name) const {
    if(!strcmp(name, vs9Win32generaorName))
      {
      return new cmGlobalVisualStudio9Generator(
        vs9Win32generaorName, NULL, NULL);
      }
    if(!strcmp(name, vs9Win64generaorName))
      {
      return new cmGlobalVisualStudio9Generator(
        vs9Win64generaorName, "x64", "CMAKE_FORCE_WIN64");
      }
    if(!strcmp(name, vs9IA64generaorName))
      {
      return new cmGlobalVisualStudio9Generator(
        vs9IA64generaorName, "Itanium", "CMAKE_FORCE_IA64");
      }
    return 0;
  }

  virtual void GetDocumentation(cmDocumentationEntry& entry) const {
    entry.Name = "Visual Studio 9 2008";
    entry.Brief = "Generates Visual Studio 9 2008 project files.";
    entry.Full =
      "It is possible to append a space followed by the platform name "
      "to create project files for a specific target platform. E.g. "
      "\"Visual Studio 9 2008 Win64\" will create project files for "
      "the x64 processor; \"Visual Studio 9 2008 IA64\" for Itanium.";
  }

  virtual void GetGenerators(std::vector<std::string>& names) const {
    names.push_back(vs9Win32generaorName);
    names.push_back(vs9Win64generaorName);
    names.push_back(vs9IA64generaorName); }
};

//----------------------------------------------------------------------------
cmGlobalGeneratorFactory* cmGlobalVisualStudio9Generator::NewFactory()
{
  return new Factory;
}

//----------------------------------------------------------------------------
cmGlobalVisualStudio9Generator::cmGlobalVisualStudio9Generator(
  const char* name, const char* architectureId,
  const char* additionalPlatformDefinition)
  : cmGlobalVisualStudio8Generator(name, architectureId,
                                   additionalPlatformDefinition)
{
  this->FindMakeProgramFile = "CMakeVS9FindMake.cmake";
}

//----------------------------------------------------------------------------
void cmGlobalVisualStudio9Generator::WriteSLNHeader(std::ostream& fout)
{
  fout << "Microsoft Visual Studio Solution File, Format Version 10.00\n";
  fout << "# Visual Studio 2008\n";
}

///! Create a local generator appropriate to this Global Generator
cmLocalGenerator *cmGlobalVisualStudio9Generator::CreateLocalGenerator()
{
  cmLocalVisualStudio7Generator *lg
    = new cmLocalVisualStudio7Generator(cmLocalVisualStudioGenerator::VS9);
  lg->SetPlatformName(this->GetPlatformName());
  lg->SetExtraFlagTable(this->GetExtraFlagTableVS8());
  lg->SetGlobalGenerator(this);
  return lg;
}

//----------------------------------------------------------------------------
void cmGlobalVisualStudio9Generator
::EnableLanguage(std::vector<std::string>const &  lang,
                 cmMakefile *mf, bool optional)
{
  cmGlobalVisualStudio8Generator::EnableLanguage(lang, mf, optional);
}

//----------------------------------------------------------------------------
std::string cmGlobalVisualStudio9Generator::GetUserMacrosDirectory()
{
  std::string base;
  std::string path;

  // base begins with the VisualStudioProjectsLocation reg value...
  if (cmSystemTools::ReadRegistryValue(
    "HKEY_CURRENT_USER\\Software\\Microsoft\\VisualStudio\\9.0;"
    "VisualStudioProjectsLocation",
    base))
    {
    cmSystemTools::ConvertToUnixSlashes(base);

    // 9.0 macros folder:
    path = base + "/VSMacros80";
      // *NOT* a typo; right now in Visual Studio 2008 beta the macros
      // folder is VSMacros80... They may change it to 90 before final
      // release of 2008 or they may not... we'll have to keep our eyes
      // on it
    }

  // path is (correctly) still empty if we did not read the base value from
  // the Registry value
  return path;
}

//----------------------------------------------------------------------------
std::string cmGlobalVisualStudio9Generator::GetUserMacrosRegKeyBase()
{
  return "Software\\Microsoft\\VisualStudio\\9.0\\vsmacros";
}
