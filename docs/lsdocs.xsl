<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
<html>
<head>
<style>
table		{ width: 100%; border-collapse: collapse; }
td		{ border: 1px solid }
h1, h3.head	{ padding: 0px; margin: 0px; }
</style>
</head>
<body>
  <xsl:for-each select="lsdoc/module">
    <table style="text-align: center">
    <tr><td style="background-color: black; color: white; border-color: black;"><h1><xsl:value-of select="@filename" /></h1></td></tr>
    <tr><td>
    <h3 class="head">By: <xsl:value-of select="author/@name" /> <span style="font-size:0.75em">
    <xsl:if test="string-length(author/@website) &gt; 0">
      (<xsl:element name="a">
        <xsl:attribute name="href">http://<xsl:value-of select="author/@website" /></xsl:attribute>
        <!-- xsl:value-of select="author/@website" / -->
        <xsl:text>homepage</xsl:text>
      </xsl:element>) 
    </xsl:if>
    <xsl:if test="string-length(author/@email) &gt; 0">
      (<xsl:element name="a">
        <xsl:attribute name="href">mailto:<xsl:value-of select="author/@email" /></xsl:attribute>
        <!-- xsl:value-of select="author/@email" / -->
        <xsl:text>email</xsl:text>
      </xsl:element>)
    </xsl:if>
    </span>
    </h3>
    <span style="font-size: 0.9em">
    <xsl:if test="string-length(author/@icq) &gt; 0">
      <b>ICQ</b>: <xsl:value-of select="author/@icq" /><br />
    </xsl:if>
    <xsl:if test="string-length(author/@msn) &gt; 0">
      <b>MSN</b>: <xsl:value-of select="author/@msn" /><br />
    </xsl:if>
    <xsl:if test="string-length(author/@yim) &gt; 0">
      <b>Y!</b>: <xsl:value-of select="author/@yim" /><br />
    </xsl:if>
    <xsl:if test="string-length(author/@aim) &gt; 0">
      <b>AIM</b>: <xsl:value-of select="author/@aim" /><br />
    </xsl:if>
    </span>
    </td></tr>
    </table><br />
    
    <xsl:if test="count(description) &gt; 0">
      <xsl:apply-templates select="description" />
    </xsl:if>
   
    <h2>Step.rc Commands for <xsl:value-of select="@filename" /></h2>
    <table>
    <xsl:for-each select="command">
      <tr><td style="width: 50%; text-align: center;">
        <xsl:value-of select="@name" />
        <xsl:apply-templates select="child::*" mode="paramtypes" />
      </td><td style="width: 50%;">
        <xsl:apply-templates select="description" />
        <xsl:if test="count(child::*[@default]) &gt; 0">
          <br /><b>Default: </b><xsl:apply-templates select="child::*[@default]" mode="defvalue" />
        </xsl:if>
      </td></tr>
    </xsl:for-each>
    </table><br />
  
    <h2>!Bang Commands for <xsl:value-of select="@filename" /></h2>
    <table>
    <xsl:for-each select="bang">
      <tr><td style="width: 50%; text-align: center;">
        !<xsl:value-of select="@name" />
      </td><td style="width: 50%;">
        <xsl:apply-templates select="description" />
      </td></tr>
    </xsl:for-each>
    </table><br />

    <!-- Environment variables :: jesus_mjjg :: 20030930 - 23h31 -->
      <xsl:if test="count(evar) &gt; 0">
      <h2>Environment Variables for <xsl:value-of select="@filename" /></h2>
      <table>
      <xsl:for-each select="evar">
        <tr><td style="width: 50%; text-align: center;">
          $<xsl:value-of select="@name" />$
        </td><td style="width: 50%;">
          <xsl:apply-templates select="description" />
        </td></tr>
      </xsl:for-each>
      </table><br />
    </xsl:if>
    
    <xsl:if test="count(changelog) &gt; 0">
      <h2>Changelog for <xsl:value-of select="@filename" /></h2>
      <table>
      <tr><td style="width: 100%;">
      <xsl:for-each select="changelog/version">
        Version <xsl:value-of select="@id" />
        <ul>
        <xsl:for-each select="entry">
          <li><xsl:value-of select="text()" /></li>
        </xsl:for-each>
        </ul>
      </xsl:for-each>
      </td></tr>
      </table>
    </xsl:if>
    <br />
    <table>
    <tr><td style="width: 100%; text-align: center;">
    <xsl:for-each select="copyright">
      <xsl:apply-templates />
    </xsl:for-each>
    </td></tr>
    </table>
  </xsl:for-each>
</body>
</html>
</xsl:template>

<xsl:template match="description" mode="paramtypes">
</xsl:template>

<xsl:template match="*" mode="defvalue">
    <xsl:value-of select="@default" /><xsl:text> </xsl:text>
</xsl:template>

<xsl:template match="*" mode="paramtypes">
  <xsl:text> [</xsl:text><xsl:value-of select="name(current())" /><xsl:text>] </xsl:text>
</xsl:template>

<xsl:template match="list" mode="paramtypes">
  <xsl:text> [</xsl:text><xsl:value-of select="@name" /><xsl:text>] </xsl:text>
</xsl:template>

<xsl:template match="description">
    <xsl:if test="string-length(@header) &gt; 0">
      <!-- Right title header :: jesus_mjjg :: 20030930 - 23h31 -->
      <!-- <h3><xsl:value-of select="@header" /></h3> -->
      <h2><xsl:value-of select="@header" /></h2>
    </xsl:if>
    <xsl:apply-templates /><br />
</xsl:template>

<xsl:template match="url">
  <xsl:element name="a">
    <xsl:attribute name="href"><xsl:value-of select="@src" /></xsl:attribute>
    <xsl:attribute name="title"><xsl:value-of select="text()" /></xsl:attribute>
    <xsl:apply-templates />
  </xsl:element>
</xsl:template>

<xsl:template match="b">
  <xsl:element name="span">
    <xsl:attribute name="style">font-weight: bold</xsl:attribute>
    <xsl:apply-templates />
  </xsl:element>
</xsl:template>

<xsl:template match="i">
  <xsl:element name="span">
    <xsl:attribute name="style">font-style: italic</xsl:attribute>
    <xsl:apply-templates />
  </xsl:element>
</xsl:template>

<xsl:template match="u">
  <xsl:element name="span">
    <xsl:attribute name="style">text-decoration: underline</xsl:attribute>
    <xsl:apply-templates />
  </xsl:element>
</xsl:template>

<xsl:template match="br">
  <xsl:element name="br" />
</xsl:template>

</xsl:stylesheet>