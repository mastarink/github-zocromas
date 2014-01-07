<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method='html'/>
<xsl:template match="/">
<html>
<head>
<title>treeline</title>
</head>
<body>
<xsl:apply-templates/>
</body>
</html>
</xsl:template>

<xsl:template match="/ROOT">
<xsl:apply-templates/>
</xsl:template>

<xsl:template match="DEFAULT[@item='y']">
<xsl:if test="normalize-space(./Name)">
<xsl:for-each select="./Name">
<a id="{.}" />
</xsl:for-each></xsl:if>
<xsl:if test="normalize-space(./Name)">
<xsl:if test="normalize-space(./Name)"><xsl:text></xsl:text><xsl:value-of select="./Name"/><xsl:text></xsl:text></xsl:if><br />
</xsl:if>
<br/>
<div style="margin-left:20px">
<xsl:apply-templates/>
</div>
</xsl:template>

<xsl:template match="ROOT[@item='y']">
<xsl:if test="normalize-space(./Name)">
<xsl:for-each select="./Name">
<a id="{.}" />
</xsl:for-each></xsl:if>
<xsl:if test="normalize-space(./Name)">
<xsl:if test="normalize-space(./Name)"><xsl:text></xsl:text><xsl:value-of select="./Name"/><xsl:text></xsl:text></xsl:if><br />
</xsl:if>
<br/>
<div style="margin-left:20px">
<xsl:apply-templates/>
</div>
</xsl:template>

<xsl:template match="*" />

</xsl:stylesheet>
