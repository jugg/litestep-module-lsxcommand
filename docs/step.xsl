<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output indent="no" method="text"/>
	<xsl:template match="/">
		<xsl:for-each select="lsdoc/module">
			<xsl:for-each select="command">
<!--				<xsl:if test="count(child::*[@default]) &gt; 0"> -->
					<xsl:value-of select="@name" />
					<xsl:text> </xsl:text>
					<xsl:apply-templates select="child::*[@default]" mode="defvalue" />
<xsl:text>
</xsl:text>
<!--				</xsl:if> -->
			</xsl:for-each>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="*" mode="defvalue">
		<xsl:value-of select="@default" />
	</xsl:template>

</xsl:stylesheet>