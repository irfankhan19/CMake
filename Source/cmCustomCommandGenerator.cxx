/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2010 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "cmCustomCommandGenerator.h"

#include "cmMakefile.h"
#include "cmCustomCommand.h"
#include "cmLocalGenerator.h"

//----------------------------------------------------------------------------
cmCustomCommandGenerator::cmCustomCommandGenerator(
  cmCustomCommand const& cc, const char* config, cmMakefile* mf):
  CC(cc), Config(config), Makefile(mf), LG(mf->GetLocalGenerator()),
  OldStyle(cc.GetEscapeOldStyle()), MakeVars(cc.GetEscapeAllowMakeVars())
{
}

//----------------------------------------------------------------------------
unsigned int cmCustomCommandGenerator::GetNumberOfCommands() const
{
  return static_cast<unsigned int>(this->CC.GetCommandLines().size());
}

//----------------------------------------------------------------------------
std::string cmCustomCommandGenerator::GetCommand(unsigned int c) const
{
  std::string const& argv0 = this->CC.GetCommandLines()[c][0];
  return this->LG->GetRealLocation(argv0.c_str(), this->Config);
}

//----------------------------------------------------------------------------
void
cmCustomCommandGenerator
::AppendArguments(unsigned int c, std::string& cmd) const
{
  cmCustomCommandLine const& commandLine = this->CC.GetCommandLines()[c];
  for(unsigned int j=1;j < commandLine.size(); ++j)
    {
    std::string const& arg = commandLine[j];
    cmd += " ";
    if(this->OldStyle)
      {
      cmd += this->LG->EscapeForShellOldStyle(arg.c_str());
      }
    else
      {
      cmd += this->LG->EscapeForShell(arg.c_str(), this->MakeVars);
      }
    }
}
