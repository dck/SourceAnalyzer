# Copyright 2008-2011 SourceAnalyzer team. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
# 
#    1. Redistributions of source code must retain the above copyright notice, this list of
#       conditions and the following disclaimer.
# 
#    2. Redistributions in binary form must reproduce the above copyright notice, this list
#       of conditions and the following disclaimer in the documentation and/or other materials
#       provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY SOURCEANALYZER TEAM ''AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SOURCEANALYZER TEAM OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# The views and conclusions contained in the software and documentation are those of the
# authors and should not be interpreted as representing official policies, either expressed
# or implied, of SourceAnalyzer team.

## This function adds specifid prefix for each item in the given list
## prefix - prefix to add
## refItems - reference to a list, witch will be processed (it's name)
macro( addPrefix prefix refItems )
	set( newItems )
	foreach( item ${${refItems}} )
		set( newItems ${newItems} ${prefix}/${item})
	endforeach( item ${${refItems}} )
	set( ${refItems} ${newItems} )
endmacro( addPrefix prefix refItems )

## This function sets COMPONENT_NAME variable used over component's cmakelists
## componentPath - path to the component, last dir-name is it's name
function( setComponentName componentPath )
	string(REPLACE "/" ";" path_list ${componentPath})
	
endfunction( setComponentName )

function( printSettingsForComponent componentName )
	message( "Dirrectories for ${componentName}: " )
	message( "    Root dir:    ${${componentName}_root_dir}"    )
	message( "    Src dir:     ${${componentName}_src_dir}"     )
	message( "    Include dir: ${${componentName}_include_dir}" )
	
	message( "Files for ${componentName}: " )
	message( "    External headers: ${${componentName}_EXTERNAL_HEADERS}" )
	message( "" )
	message( "    Internal headers: ${${componentName}_INTERNAL_HEADERS}" )
	message( "" )
	message( "    Sorces:           ${${componentName}_SRC}"              )
	message( "" )
	message( "    Output name:      ${${componentName}_OUTPUT_NAME}"      )
endfunction( printSettingsForComponent componentName )

#function(hello MESSAGE)
#    message(${MESSAGE})
#endfunction(hello) 

