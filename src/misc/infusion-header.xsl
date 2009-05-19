<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method='html' version='1.0' encoding='UTF-8' indent='yes'/>

  

  <xsl:template match="infusion">
    <xsl:for-each select="header">
          <h2>HEADER</h2>
          <div class="indent">
            <div>Name: <xsl:value-of select="@name"/></div>
            <div>Version: <xsl:value-of select="@majorversion"/>.<xsl:value-of select="@minorversion"/></div>
            <div>Entrypoint: <xsl:value-of select="@entrypoint"/></div>
          </div>
        </xsl:for-each>
        <xsl:for-each select="classlist">
          <h2>CLASSLIST</h2>
          <div class="indent">
            <xsl:for-each select="classdef">
              <h3>CLASSDEF (<xsl:value-of select="../../header/@name"/>.<xsl:value-of select="@entity_id"/>) </h3>
              <div class="indent">
                <div>Name: <xsl:value-of select="@name"/></div>
                <div>Superclass: <xsl:value-of select="@superclass.name"/>
                  (<xsl:value-of select="@superclass.infusion"/>.<xsl:value-of select="@superclass.entity_id"/>)
                </div>

                <xsl:if test="count(method) &gt; 0" >
                  <h4>Methods</h4>
                  <ul class="methodlist" >
                    <xsl:for-each select="method">
                      <li>
                        def(<xsl:value-of select="@methoddef.infusion"/>.<xsl:value-of select="@methoddef.entity_id"/>)
                        -&gt;
                        impl(<xsl:value-of select="@methodimpl.infusion"/>.<xsl:value-of select="@methodimpl.entity_id"/>)
                      </li>
                    </xsl:for-each>
                  </ul>
                </xsl:if>
                <xsl:if test="count(fielddef) &gt; 0" >
                  <h4>Fields</h4>
                  <ul class="fieldlist" >
                    <xsl:for-each select="fielddef">
                      <li>
                        <!-- This attribute seams meaningless (0.<xsl:value-of select="@entity_id"/>): -->
                        <xsl:value-of select="@type"/>&#160;
                        <xsl:value-of select="@name"/>
                        <xsl:if test="@type = 'Ref'">
                          signature: <xsl:value-of select="@signature"/>
                        </xsl:if>
                      </li>
                    </xsl:for-each>
                  </ul>
                </xsl:if>

              </div>
            </xsl:for-each>
          </div>
        </xsl:for-each>

        <xsl:for-each select="methoddeflist">
          <h2>METHODDEFLIST</h2>
          <div class="indent">
            <xsl:for-each select="methoddef">
              <h3>METHODDEF (<xsl:value-of select="../../header/@name"/>.<xsl:value-of select="@entity_id"/>)</h3>
              <div class="indent">
                <div>Name: <xsl:value-of select="@name"/> </div>
                <div>Signature: <xsl:value-of select="@signature"/></div>
              </div>
            </xsl:for-each>
          </div>
        </xsl:for-each>

        <xsl:for-each select="methodimpllist">
          <h2>METHODIMPLLIST</h2>
          <div class="indent">
            <xsl:for-each select="methodimpl">
              <h3>METHODIMPL (<xsl:value-of select="../../header/@name"/>.<xsl:value-of select="@entity_id"/>)</h3>
              <div class="indent">
                <div>Definition: (<xsl:value-of select="@methoddef.infusion"/>.<xsl:value-of select="@methoddef.entity_id"/>) </div>
                <div>Parent class: (<xsl:value-of select="@parentclass.infusion"/>.<xsl:value-of select="@parentclass.entity_id"/>) </div>
              </div>
            </xsl:for-each>

          </div>
        </xsl:for-each>

        <xsl:for-each select="staticfieldlist">
          <h2>STATICFIELDLIST</h2>
          <div class="indent">
            <xsl:for-each select="fielddef">
              <div>
                <h3>STATIC FIELD 
                  (<xsl:value-of select="@parentclass.infusion"/>.<xsl:value-of select="@entity_id"/>)
                  in class 
                  (<xsl:value-of select="@parentclass.infusion"/>.<xsl:value-of select="@parentclass.entity_id"/>)</h3>
                <div class="indent">
                  <xsl:value-of select="@type"/> &#160;
                  <xsl:value-of select="@name"/>
                  <xsl:if test="@type = 'Ref'">
                    signature: <xsl:value-of select="@signature"/>
                  </xsl:if>
                  
                </div>
              </div>
            </xsl:for-each>
          </div>
        </xsl:for-each>

        <xsl:for-each select="stringtable">
          <h2>STRINGTABLE</h2>
          <div class="indent">
            <xsl:for-each select="string">
              <div>
                <h3>STRING (<xsl:value-of select="../../header/@name"/>.<xsl:value-of select="@entity_id"/>)</h3>
                <div class="indent">
                  value: <xsl:value-of select="@value" />
                </div>
            </div>
            </xsl:for-each>
          </div>
        </xsl:for-each>

  </xsl:template>


  <xsl:template match="/dih">
    <html>
      <head>
        <style type="text/css">
*{
    margin:0;
    padding:0;
}


.methodlist, .fieldlist
{
    margin-left:2em;
}

.indent
{
    margin-left:4em;
    padding-left:.3ex;
    border-left:1px solid grey;
}

	</style>


      </head>

      <body>
        <h1>Infusion <xsl:value-of select="infusion/header/@name" /></h1>

        <xsl:apply-templates/>
        

        <xsl:for-each select="infusion/infusionlist">
          <h2>INFUSIONLIST</h2>
          <div class="indent">
            <xsl:apply-templates/>
          </div>

        </xsl:for-each>

        
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
