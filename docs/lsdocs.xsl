<!-- **************************************************************************************************************** -->
<xsl:stylesheet  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  version="1.0">
<xsl:output method="html" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" indent="yes"/>
    <xsl:template match="/lsmd">
        <html>
            <head>
                    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
                <link type="text/css" href="docs.css" rel="stylesheet"/>
                <title>
                    <xsl:value-of select="module/@filename"/> v<xsl:value-of select="module/@version"/>
                </title>
            </head>
            <body>
<!-- -->
                <table width="100%" border="0">
                    <tr class="border">
                        <td>
                            <table width="100%" border="0" cellpadding="0" cellspacing="0" class="main">
                                <tr>
                                    <td>
                                        <table width="100%" border="0" cellpadding="10" cellspacing="0">
                                            <tr>
                                                <td>
                                                    <div class="title">
                                                        <xsl:value-of select="module/@filename"/> documentation
                                                    </div>
                                                    <span class="info">
                                                        XML/XSL-generated documentation, on the official litestep docs model.<br/>
                                                        docs layout written by the LiteStep Documentation Effort, August 2000<br/>
                                                        XML schema by <A HREF="mailto:mrmidi__AT__gmx__DOT__net">MrMidi</A>, XSL by <A HREF="mailto:jesus_mjjg__AT__skynet__DOT__be">jesus_mjjg</A><br/>
                                                    </span>
                                                </td>
                                                <td align="right" valign="middle">
                                                    <img src="images/logo.gif" alt=""/>
                                                </td>
                                            </tr>
                                        </table>
                                    </td>
                                </tr>
                            <tr>
                        <td>
<!-- -->
        <div class="head">What does <xsl:value-of select="module/@filename"/> do?</div>
        <div class="body"><xsl:value-of select="module/description"/><br/><br/></div>
                <!-- -->
        <div class="head">Step.rc syntax for <xsl:value-of select="module/@filename"/>:</div>
                <!-- -->
                <xsl:apply-templates select="module/command-group"/>
                <xsl:apply-templates select="module/bangs"/>
                <!-- -->
                <div class="head">
                About <xsl:value-of select="module/@filename"/> v<xsl:value-of select="module/@version"/>:<br/>
                </div>
                <div class="body">
                    Author contact informations:<br/>
                    Name: <xsl:value-of select="module/author/@name"/><br/>
                    <xsl:if test="string(module/author/@email)">
                        E-mail:<a><xsl:attribute name="href">mailto:<xsl:value-of select="module/author/@email"/>
                        </xsl:attribute><xsl:value-of select="module/author/@email"/></a><br/>
                    </xsl:if>
                    <xsl:if test="string(module/author/@icq)">
                        Icq: <xsl:value-of select="module/author/@icq"/><br/>
                    </xsl:if>
                    <xsl:if test="string(module/author/@msn)">
                        Msn: <xsl:value-of select="module/author/@msn"/><br/>
                    </xsl:if>
                    <xsl:if test="string(module/author/@yim)">
                        Yim: <xsl:value-of select="module/author/@yim"/><br/>
                    </xsl:if>
                    <xsl:if test="string(module/author/@aim)">
                        Aim: <xsl:value-of select="module/author/@aim"/><br/>
                    </xsl:if>
                    <xsl:if test="string(module/author/@url)">
                        Homepage: <a>
                                      <xsl:attribute name="href">
                                          <xsl:value-of select="module/author/@url"/>
                                      </xsl:attribute>
                                      <xsl:value-of select="module/author/@url"/>
                                  </a><br/>
                    </xsl:if>
                    <xsl:if test="string(module/url/@src)">
                        Project homepage: <a>
                                              <xsl:attribute name="href">
                                                  <xsl:value-of select="module/url/@src"/>
                                              </xsl:attribute>
                                              <xsl:value-of select="module/url/@src"/>
                                          </a><br/>

                    </xsl:if>
                </div>
                <!-- -->
</td>
</tr>

        <tr class="body">
            <td valign="bottom" align="right">
                <div align="right">
                    <img src="images/valid-html401.gif" alt="valid-html401"/>
                </div>
            </td>
        </tr>
<!--    </tbody>-->
</table>
</td>
</tr>
</table>

            </body>
        </html>        
    </xsl:template>
<!-- ****************************************************************************************************************
     * Here are the templates                                                                                       *
     **************************************************************************************************************** -->
    <xsl:template match="module">
        <xsl:apply-templates/>
    </xsl:template>
<!-- **************************************************************************************************************** -->
    <xsl:template match="command-group">
        <div class="head2">
            <xsl:value-of select="@name"/>
        </div>
        <table width="100%" cellspacing="0" cellpadding="0" border="1" align="center">
            <tr class="border">
                <td>
                    <table width="100%" border="1" cellpadding="10" cellspacing="0">
                        <xsl:apply-templates/>
                    </table>
                </td>
            </tr>
        </table>
    </xsl:template>
<!-- **************************************************************************************************************** -->
    <xsl:template match="command">
        <tr class="item">
            <td align="center" class="data" width="40%">
        <code>
                    <xsl:value-of select="@name"/>
        </code>
            </td>
            <td class="data" width="60%">
                <xsl:value-of select="description"/>
            </td>
        </tr>
    </xsl:template>
<!-- **************************************************************************************************************** -->
    <xsl:template match="bangs">
        <h2>Bang commands:</h2>
        <table width="100%" cellspacing="0" cellpadding="0" border="1" align="center">
            <tr class="border">
                <td>
                    <table width="100%" border="1" cellpadding="10" cellspacing="0">
                        <tbody>
                            <!--<xsl:apply-templates/>-->
                            <xsl:apply-templates select="./bang"/>
                        </tbody>
                    </table>
                </td>
            </tr>
        </table>
    </xsl:template>
<!-- **************************************************************************************************************** -->
    <xsl:template match="bang">
        <tr class="item">
            <td align="center" class="data" width="40%">
                <code>
                    !<xsl:value-of select="@name"/>
                </code>
            </td>
            <td class="data" width="60%">
                <xsl:value-of select="description"/>
            </td>
        </tr>
    </xsl:template>
<!-- **************************************************************************************************************** -->
    <xsl:template match="description">
        <div class="body">
                <xsl:value-of select="."/>
        </div>
    </xsl:template>
<!-- **************************************************************************************************************** -->
    <xsl:template match="author">
        </xsl:template>
    </xsl:stylesheet>
<!-- **************************************************************************************************************** -->
<!-- **************************************************************************************************************** -->
<!--
        "<xsl:text/>
            <xsl:choose>
                <xsl:when test='@CAT="F"'>Fiction</xsl:when>
                <xsl:when test='@CAT="S"'>Science</xsl:when>
                <xsl:when test='@CAT="C"'>Computing</xsl:when>
                <xsl:when test='@CAT="X"'>Crime</xsl:when>
                <xsl:otherwise>Unclassified</xsl:otherwise>
            </xsl:choose>)
        "<xsl:text/>

-->